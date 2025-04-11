modded class MissionGameplay extends MissionBase
{
	protected const float RPC_SEND_INTERVAL = 60.0;
	protected float m_LastRPCSentTimestamp = 0;

	autoptr EarPlugsWidget m_earplugswidget = new EarPlugsWidget;
    protected ref LeaderboardWidget m_LeaderboardUI;
	protected ref RespawnDialogue m_RespawnDialogue;
	protected ref GameMasterUI m_GameMasterUI;
	protected ref TeamSelectUI m_TeamSelectUI;
	protected ref CompassWidget m_CompassUI;
	protected ref CurrencyUI m_CurrencyUI;
	protected ref EventStateUI m_StateUI;
	protected ref BuyMenuUI m_BuyMenuUI;
	protected ref KOTHWidget m_KOTHUI;
	protected ref SnDWidget m_SnDUI;

	string m_Mode;
	int m_State;
	float m_Timer;
	int m_CurrentRound;
	int m_UntilSwitch;
	vector m_SiteA;
	vector m_SiteB;
	int m_ArmedSite;

	bool m_InitUI = false;
	int m_EarPlugsState = 0;
	bool m_ChatChannelGlobal = true;
	private int m_PreviousStatLevel = -1;
	private bool m_StatLevelInitialized = false;

	int m_StatLevel;
	int m_StatScore;
	int m_StatCurrency;
	int m_StatPKills;
	int m_StatZKills;
	int m_StatDeaths;

	int m_BuyZoneRestrict;
	bool m_NeedsTeam = false;
	bool m_LeavingTeam = false;
	bool m_CanVote = false;
	ref array<vector> m_BuyZones;
	ref array<int> m_SavedLoadout = { 0, 0, 0, 0, 0, -1, -1, -1 };
	private string m_SavedCartData;

	ref array<ref BGPlayerStats> m_StoredLeaderboardData = new array<ref BGPlayerStats>();
	ref array<ref PlayerTeamData> m_StoredPlayerList = new array<ref PlayerTeamData>();
	ref map<string, int> m_TeamScores = new map<string, int>();

// ================================= GAME MASTER =============================================

	private bool m_IsGameMaster = false;
	void ReceiveGameMaster(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param1<bool> param;
		if (!ctx.Read(param))
			return;

		if (type == CallType.Client)
		{
			m_IsGameMaster = param.param1;
		}
	}

// ================================= MAP =============================================

	void HandleMapUpdate()
	{
		UIManager um = GetGame().GetUIManager();
		if(um && um.IsMenuOpen(MENU_MAP))
		{
			MapMenu mapMenu = MapMenu.Cast(um.GetMenu());
			if (mapMenu)
			{
				mapMenu.ClearEventMarkers();
				mapMenu.AddBuyZones(m_BuyZones);
				mapMenu.AddEventPos(m_SiteA, m_SiteB, m_ArmedSite);
			}
		}
	}

	override protected void HandleMapToggleByKeyboardShortcut(Man player)
	{
		UIManager um = GetGame().GetUIManager();
		if (um && !um.IsMenuOpen(MENU_MAP))
		{
			um.CloseAll();
			if (!CfgGameplayHandler.GetUse3DMap())
			{
				um.EnterScriptedMenu(MENU_MAP, null);
				MapMenu mapMenu = MapMenu.Cast(um.GetMenu());
				if (mapMenu)
				{
					mapMenu.AddBuyZones(m_BuyZones);
					mapMenu.AddEventPos(m_SiteA, m_SiteB, m_ArmedSite);
				}

				GetGame().GetMission().AddActiveInputExcludes({"map"});
				GetGame().GetMission().AddActiveInputRestriction(EInputRestrictors.MAP);
			}
		}
	}

	void CloseMap()
	{
		if (m_UIManager.IsMenuOpen(MENU_MAP))
		{
			UIManager um = GetGame().GetUIManager();
			MapMenu mapMenu = MapMenu.Cast(um.GetMenu());
			if (mapMenu)
				mapMenu.CloseMapMenu();
		}
	}

// ================================= INIT =============================================

	void MissionGameplay()
	{
		GetRPCManager().AddRPC("Battlegrounds", "ReceiveEventState", this, SingleplayerExecutionType.Client);
		GetRPCManager().AddRPC("Battlegrounds", "ReceiveSelectRequest", this, SingleplayerExecutionType.Client);
		GetRPCManager().AddRPC("Battlegrounds", "ReceivePlayerList", this, SingleplayerExecutionType.Client);
		GetRPCManager().AddRPC("Battlegrounds", "ReceiveLeaderboard", this, SingleplayerExecutionType.Client);
		GetRPCManager().AddRPC("Battlegrounds", "ReceiveCurrency", this, SingleplayerExecutionType.Client);
		GetRPCManager().AddRPC("Battlegrounds", "ReceiveStats", this, SingleplayerExecutionType.Client);
        GetRPCManager().AddRPC("Battlegrounds", "ReceiveKOTH", this, SingleplayerExecutionType.Client);
		GetRPCManager().AddRPC("Battlegrounds", "ReceiveBuyZones", this, SingleplayerExecutionType.Client);
		GetRPCManager().AddRPC("Battlegrounds", "ReceiveScore", this, SingleplayerExecutionType.Client);
		GetRPCManager().AddRPC("Battlegrounds", "ReceiveSnD", this, SingleplayerExecutionType.Client);
		GetRPCManager().AddRPC("Battlegrounds", "ReceiveGameMaster", this, SingleplayerExecutionType.Client);
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();

		if (!m_CurrencyUI)
		{
			m_CurrencyUI = new CurrencyUI();
			m_CurrencyUI.Init();
		}

		if (!m_StateUI)
		{
			m_StateUI = new EventStateUI();
			m_StateUI.Init();
		}
	}

// ================================= STATE =================================

	void ReceiveEventState(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param3<string, int, float> param;
		if (!ctx.Read(param))
			return;
		
		if (type == CallType.Client)
		{
			m_Mode = param.param1;
			m_State = param.param2;
			m_Timer = param.param3;

			if (m_State == STATE_CLEANUP)
			{
				CloseMap();
				ResetEventUI();
				UpdateStateSnD();
			}
			else
			{
				InitEventUI();
				UpdateStateSnD();
			}

			if (m_Mode == MODE_DAYZ)
			{
				RemoveModdedUI();
			}
		}
	}

	void RemoveModdedUI()
	{
		m_NeedsTeam = false;

		if (m_CompassUI)
		{
			m_CompassUI.CloseUI();
			m_CompassUI = null;
		}

		if (m_TeamSelectUI)
		{
			m_TeamSelectUI.CloseTeamSelect();
			m_TeamSelectUI = null;
		}
	}

	void ResetEventUI()
	{
		m_StateUI.StopSZCheck();

		if (m_KOTHUI)
		{
			m_KOTHUI.CloseUI();
			m_KOTHUI = null;
		}

		if (m_SnDUI)
		{
			m_SnDUI.CloseUI();
			m_SnDUI = null;
		}

		if (m_CompassUI)
		{
			m_CompassUI.CleanupCompass();
		}
	}

	void InitEventUI()
	{
		if (!m_CompassUI && m_Mode != MODE_DAYZ)
		{
			m_CompassUI = new CompassWidget();
			m_CompassUI.Init();
		}

		if (!m_KOTHUI && m_Mode == MODE_KOTH)
		{
			m_StateUI.InitSZCheck();
			m_KOTHUI = new KOTHWidget();
			m_KOTHUI.Init();
		}

		if (!m_SnDUI && m_Mode == MODE_SND)
		{
			m_SnDUI = new SnDWidget();
			m_SnDUI.Init();
		}
	}

	void UpdateStateSnD()
	{
		if (!m_State)
			return;

		if ((m_State == SND_STATE_ACTIVE) && m_Timer >= 0)
		{
			m_StateUI.SetTime(m_Timer);
			m_StateUI.ShowState(true, SND_STATE_ACTIVE, m_CurrentRound);
		}
		else if ((m_State == SND_STATE_ARMED) && m_Timer >= 0)
		{
			m_StateUI.SetTime(m_Timer);
			m_StateUI.ShowState(true, SND_STATE_ARMED);
		}
		else if ((m_State == SND_STATE_ACTIVE_BUY) && m_Timer >= 0)
		{
			m_StateUI.SetTime(m_Timer);
			m_StateUI.ShowState(true, SND_STATE_ACTIVE_BUY);
		}
		else
		{
			m_StateUI.HideTimer();
			m_StateUI.ShowState(false, m_State);
		}
	}

// ================================= SnD =============================================

	void ReceiveSnD(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param5<int, int, vector, vector, int> param;
		if (!ctx.Read(param))
			return;
		
		if (type == CallType.Client)
		{
			m_CurrentRound = param.param1;
			m_UntilSwitch = param.param2;
			m_SiteA = param.param3;
			m_SiteB = param.param4;
			m_ArmedSite = param.param5;
			HandleMapUpdate();

			if (m_State == SND_STATE_ACTIVE && m_StateUI && m_CurrentRound > 0)
			{
				m_StateUI.ShowState(true, SND_STATE_ACTIVE, m_CurrentRound);
			}

			if (m_CompassUI)
				m_CompassUI.SetSnDCoords(m_SiteA, m_SiteB, m_ArmedSite);
		}
	}

// ================================= TEAM SELECT =============================================

	void ReceiveSelectRequest(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param1<int> param;
		if (!ctx.Read(param))
			return;

		if (type == CallType.Client)
		{
			if (param.param1 == REQUEST_TEAMSELECT)
			{
				m_NeedsTeam = true;
				m_LeavingTeam = false;
				if (!m_TeamSelectUI)
				{
					m_TeamSelectUI = new TeamSelectUI();
					m_TeamSelectUI.Init();
					m_TeamSelectUI.SetTeams(m_Mode);
				}
				else
				{
					m_TeamSelectUI.HideTeams();
					m_TeamSelectUI.SetTeams(m_Mode);
				}
			}
			else if (param.param1 == REQUEST_VOTE)
			{
				SetVoting(true);
			}
			else if(param.param1 == CLOSE_VOTE)
			{
				SetVoting(false);
			}
		}
	}

	bool CanVote()
	{
		return m_CanVote;
	}

	void SetVoting(bool canVote)
	{
		if (canVote)
		{
			m_CanVote = true;
		}
		else 
		{
			m_CanVote = false;
		}
	}

// ================================= LEADERBOARD =============================================

	void ReceiveScore(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param1<array<ref Param2<string, int>>> param;
		if (!ctx.Read(param))
			return;
		
		if (type == CallType.Client) 
		{
			m_TeamScores.Clear();
			foreach (Param2<string, int> teamScore : param.param1)
			{
				m_TeamScores.Insert(teamScore.param1, teamScore.param2);
			}

			if (m_LeaderboardUI)
				m_LeaderboardUI.UpdatePlayerList(m_StoredPlayerList, m_TeamScores);
		}
	}

	void ReceivePlayerList(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
	{
		ref Param1<array<ref PlayerTeamData>> data;
		if (!ctx.Read(data))
			return;

		if (type == CallType.Client)
		{
			ref array<ref PlayerTeamData> playerListData = data.param1;
			m_StoredPlayerList = playerListData;

			if (m_LeaderboardUI)
				m_LeaderboardUI.UpdatePlayerList(m_StoredPlayerList, m_TeamScores);

			if (m_Mode == MODE_SND && m_SnDUI)
			{
				int blueTeamCount = 0;
				int redTeamCount = 0;

				foreach (PlayerTeamData player : playerListData)
				{
					if (player.IsAlive())
					{
						if (player.GetTeam() == TEAM_BLUE)
							blueTeamCount++;

						else if (player.GetTeam() == TEAM_RED)
							redTeamCount++;
					}
				}

				m_SnDUI.SetPlayerCount(blueTeamCount, redTeamCount);
			}
		}
	}

	void ReceiveLeaderboard(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
	{
		Param1<array<ref BGPlayerStats>> data;
		if (!ctx.Read(data)) 
			return;
		
		if (type == CallType.Client)
		{
			ref array<ref BGPlayerStats> leaderboardData = data.param1;
			m_StoredLeaderboardData = leaderboardData;

			if(m_LeaderboardUI)
				m_LeaderboardUI.UpdateLeaderboardData(m_StoredLeaderboardData);
		}
	}

// ================================= PLAYER STATS =============================================

	void ReceiveCurrency(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param1<int> param;
		if (!ctx.Read(param))
			return;

		if (type == CallType.Client)
		{
			m_StatCurrency = param.param1;
			if (m_UIManager.IsMenuOpen(MENU_BUY))
			{
				BuyMenuUI buyMenu = BuyMenuUI.Cast(m_UIManager.GetMenu());
				if (buyMenu)
					buyMenu.SetCurrency(m_StatCurrency);
			}

			if (m_UIManager.IsMenuOpen(MENU_TRANSFER))
			{
				TransferMenuUI transferMenu = TransferMenuUI.Cast(m_UIManager.GetMenu());
				if (transferMenu)
					transferMenu.SetCurrency(m_StatCurrency);
			}

			if (m_CurrencyUI)
				m_CurrencyUI.UpdateCurrencyDisplay(m_StatCurrency);
		}
	}

	void ReceiveStats(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param5<int, int, int, int, int> param;
		if (!ctx.Read(param))
			return;
		
		if (type == CallType.Client) 
		{
			int previousLevel = m_StatLevel;

			m_StatPKills = param.param1;
			m_StatZKills = param.param2;
			m_StatDeaths = param.param3;
			m_StatScore = param.param4;
			m_StatLevel = param.param5;

			if (!m_StatLevelInitialized)
				m_StatLevelInitialized = true;

			else if (m_StatLevel != previousLevel)
			{
				string notifyNewLevel = "LEVEL: " + m_StatLevel.ToString();
				NotificationSystem.AddNotificationExtended(NOTIFY_LENGTH, NOTIFY_PROMOTION, notifyNewLevel, ICON_PROMOTION);
			}

			m_PreviousStatLevel = m_StatLevel;
		}
	}

// ================================= KOTH =============================================

    void ReceiveKOTH(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param5<string, float, int, string, vector> eventData;
		if(!ctx.Read(eventData))
			return;
		
		if (type == CallType.Client)
		{
			m_SiteA = eventData.param5;
			m_SiteB = vector.Zero;
			HandleMapUpdate();
        	UpdateKOTHWidget(eventData.param1, eventData.param2, eventData.param3, eventData.param4);
		}
    }

    void UpdateKOTHWidget(string locationName, float captureProgress, int playerCount, string dominantFaction)
	{
		if (m_KOTHUI)
		{
			m_KOTHUI.SetLocation(locationName);
            m_KOTHUI.SetProgress(captureProgress);
			m_KOTHUI.SetPlayerCount(playerCount);
			m_KOTHUI.SetDominantFaction(dominantFaction);
		}

		if (m_CompassUI)
			m_CompassUI.SetKOTHCoords(m_SiteA);
	}

	void ReceiveBuyZones(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param1<array<vector>> param;
		if (!ctx.Read(param))
			return;
		
		if (type == CallType.Client) 
		{
			m_BuyZones = param.param1;
			HandleMapUpdate();
			if (m_CompassUI)
				m_CompassUI.SetBuyZones(m_BuyZones);
		}
	}

// ================================= KEY PRESS =============================================

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		if (GetUApi() && (m_UIManager.IsMenuOpen(MENU_CHAT_INPUT) || m_UIManager.IsMenuOpen(MENU_INGAME) || m_UIManager.IsMenuOpen(MENU_INVENTORY) || m_UIManager.IsMenuOpen(MENU_MAP))) 
			return;

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player || !player.IsAlive() || player.IsUnconscious())
			return;

		if (GetGame().GetInput().LocalPress("UAGameMaster") && m_IsGameMaster)
		{
			PrepareForMenu(MENU_GAMEMASTER);

			if (m_UIManager.IsMenuOpen(MENU_GAMEMASTER))
				m_UIManager.CloseMenu(MENU_GAMEMASTER);

			else
			{
				m_GameMasterUI = GameMasterUI.Cast(m_UIManager.ShowScriptedMenu(new GameMasterUI(), null));
			}
		}

		if (GetGame().GetInput().LocalPress("UABuyMenuToggle"))
		{
			int status = player.GetSafezoneStatus();
			if (player && (status != NO_ZONE && status != SZ_LEAVING))
			{
				PrepareForMenu(MENU_BUY);

				if (m_UIManager.IsMenuOpen(MENU_BUY))
					m_UIManager.CloseMenu(MENU_BUY);

				else
				{
					if (m_Mode == MODE_KOTH && (status == SZ_INSIDE || status == SPAWN_PROTECTION))
					{
						m_BuyZoneRestrict = BUYMENU_RESTRICT_SAFEZONE;
						m_BuyMenuUI = BuyMenuUI.Cast(m_UIManager.ShowScriptedMenu(new BuyMenuUI(), null));
						m_BuyMenuUI.SetCurrency(m_StatCurrency);
						m_BuyMenuUI.SetBuyRestrictions(BUYMENU_RESTRICT_SAFEZONE, m_StatLevel);
					}
					else
					{
						m_BuyZoneRestrict = BUYMENU_RESTRICT_NONE;
						m_BuyMenuUI = BuyMenuUI.Cast(m_UIManager.ShowScriptedMenu(new BuyMenuUI(), null));
						m_BuyMenuUI.SetCurrency(m_StatCurrency);
						m_BuyMenuUI.SetBuyRestrictions(BUYMENU_RESTRICT_NONE, m_StatLevel);
					}
				}
			}

			if (player && m_Mode == MODE_SND && m_State == SND_STATE_ACTIVE_BUY)
			{
				PrepareForMenu(MENU_BUY);

				if (m_UIManager.IsMenuOpen(MENU_BUY))
					m_UIManager.CloseMenu(MENU_BUY);

				else
				{
					m_BuyMenuUI = BuyMenuUI.Cast(m_UIManager.ShowScriptedMenu(new BuyMenuUI(), null));
					m_BuyMenuUI.SetCurrency(m_StatCurrency);
					m_BuyMenuUI.SetBuyRestrictions(BUYMENU_RESTRICT_NONE, m_StatLevel);
				}
			}
		}

		if (GetGame().GetInput().LocalPress("UALeaderboardToggle"))
        {
            float currentTime = GetGame().GetTime();
            if ((currentTime - m_LastRPCSentTimestamp) > RPC_SEND_INTERVAL * 1000)
            {
                GetRPCManager().SendRPC("Battlegrounds", "LeaderboardSync", new Param, true, null); 
                m_LastRPCSentTimestamp = currentTime;
            } 

			PrepareForMenu(MENU_LEADERBOARD);

			if (m_UIManager.IsMenuOpen(MENU_LEADERBOARD))
				m_UIManager.CloseMenu(MENU_LEADERBOARD);

			else
			{
				m_LeaderboardUI = LeaderboardWidget.Cast(m_UIManager.ShowScriptedMenu(new LeaderboardWidget(), null));
				m_LeaderboardUI.SetPlayerStats(m_StatLevel, m_StatScore, m_StatCurrency, m_StatPKills, m_StatZKills, m_StatDeaths);
				m_LeaderboardUI.UpdateLeaderboardData(m_StoredLeaderboardData);
				m_LeaderboardUI.UpdatePlayerList(m_StoredPlayerList, m_TeamScores);
			}
        }

		if (GetGame().GetInput().LocalPress("UACompassToggle") && GetGame().GetUIManager().GetMenu() == NULL)
		{
			ToggleCompass();
		}

		if (GetGame().GetInput().LocalPress("UAEventToggle") && GetGame().GetUIManager().GetMenu() == NULL)
		{
			if (m_Mode == MODE_KOTH)
				ToggleKOTH();

			if (m_Mode == MODE_SND)
				ToggleSnD();
		}

		if (GetGame().GetInput().LocalPress("UAGlobalChat") && GetGame().GetUIManager().GetMenu() == NULL)
		{
			m_ChatChannelGlobal = !m_ChatChannelGlobal;
			GetGame().Chat("Channel switched to " + GetChatChannelName(), "colorAction");
		}

        if (GetGame().GetInput().LocalPress("UAEarPlugsToggle") && GetGame().GetUIManager().GetMenu() == NULL) 
		{
			if (m_EarPlugsState == 0)
			{
				m_EarPlugsState++;
				GetGame().GetSoundScene().SetSoundVolume(0.20,1);
				m_earplugswidget.SetIcon("Battlegrounds\\GUI\\volume_low.edds");
			}
			else if (m_EarPlugsState == 1)
			{
				m_EarPlugsState = 0;
				GetGame().GetSoundScene().SetSoundVolume(1,1);
				m_earplugswidget.SetIcon("Battlegrounds\\GUI\\volume_full.edds");
			}
		}
    }

// ================================= QUICK CLOSE =============================================

	void PrepareForMenu(int menu)
	{
		if (menu != MENU_LEADERBOARD && m_UIManager.IsMenuOpen(MENU_LEADERBOARD))
        	m_UIManager.CloseMenu(MENU_LEADERBOARD);

		if (menu != MENU_BUY && m_UIManager.IsMenuOpen(MENU_BUY))
			m_UIManager.CloseMenu(MENU_BUY);

		if (menu != MENU_TRANSFER && m_UIManager.IsMenuOpen(MENU_TRANSFER))
			m_UIManager.CloseMenu(MENU_TRANSFER);

		if (menu != MENU_GAMEMASTER && m_UIManager.IsMenuOpen(MENU_GAMEMASTER))
			m_UIManager.CloseMenu(MENU_GAMEMASTER);
	}

	void CloseBattlegroundsMenus()
	{
		if (m_UIManager.IsMenuOpen(MENU_LEADERBOARD))
			m_UIManager.CloseMenu(MENU_LEADERBOARD);

		if (m_UIManager.IsMenuOpen(MENU_BUY))
			m_UIManager.CloseMenu(MENU_BUY);

		if (m_UIManager.IsMenuOpen(MENU_TRANSFER))
			m_UIManager.CloseMenu(MENU_TRANSFER);

		if (m_UIManager.IsMenuOpen(MENU_GAMEMASTER))
			m_UIManager.CloseMenu(MENU_GAMEMASTER);
	}

    override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);

		if ((key == KeyCode.KC_W) || (key == KeyCode.KC_A) || (key == KeyCode.KC_S) || (key == KeyCode.KC_D))
		{
			CloseBattlegroundsMenus();
		}

		if (key == KeyCode.KC_ESCAPE)
		{
			CloseBattlegroundsMenus();
			UIScriptedMenu activeMenu = GetGame().GetUIManager().GetMenu();
			if (activeMenu && activeMenu.GetID() == MENU_RESPAWN_DIALOGUE)
			{
				PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
				if (player)
					player.ShowDeadScreen(true, 0);
			}
		}

		if (key == KeyCode.KC_F1 && m_TeamSelectUI)
		{
			CloseBattlegroundsMenus();
			m_TeamSelectUI.CloseTeamSelect();
			m_TeamSelectUI = null;
			m_NeedsTeam = false;
			RequestTeam(TEAM_BLUE);
		}

		if (key == KeyCode.KC_F2 && m_TeamSelectUI)
		{
			CloseBattlegroundsMenus();
			m_TeamSelectUI.CloseTeamSelect();
			m_TeamSelectUI = null;
			m_NeedsTeam = false;
			RequestTeam(TEAM_RED);
		}

		if (key == KeyCode.KC_F3 && m_TeamSelectUI && m_Mode == MODE_KOTH)
		{
			CloseBattlegroundsMenus();
			m_TeamSelectUI.CloseTeamSelect();
			m_TeamSelectUI = null;
			m_NeedsTeam = false;
			RequestTeam(TEAM_GREEN);
		}

		if (key == KeyCode.KC_F4 && m_TeamSelectUI && m_Mode == MODE_KOTH)
		{
			CloseBattlegroundsMenus();
			m_TeamSelectUI.CloseTeamSelect();
			m_TeamSelectUI = null;
			m_NeedsTeam = false;
			RequestTeam(TEAM_ORANGE);
		}

		if (key == KeyCode.KC_F5 && m_TeamSelectUI && m_Mode == MODE_KOTH)
		{
			CloseBattlegroundsMenus();
			m_TeamSelectUI.CloseTeamSelect();
			m_TeamSelectUI = null;
			m_NeedsTeam = false;
			RequestTeam(TEAM_YELLOW);
		}

		if (key == KeyCode.KC_F6 && m_TeamSelectUI && m_Mode == MODE_KOTH)
		{
			CloseBattlegroundsMenus();
			m_TeamSelectUI.CloseTeamSelect();
			m_TeamSelectUI = null;
			m_NeedsTeam = false;
			RequestTeam(TEAM_PINK);
		}

		if (key == KeyCode.KC_F7 && m_TeamSelectUI && m_Mode == MODE_KOTH)
		{
			CloseBattlegroundsMenus();
			m_TeamSelectUI.CloseTeamSelect();
			m_TeamSelectUI = null;
			m_NeedsTeam = false;
			RequestTeam(NO_TEAM);
		}
	}

	void RequestTeam(string team, bool leavingTeam = false)
	{
		GetRPCManager().SendRPC("Battlegrounds", "RequestTeam", new Param2<string, bool>(team, leavingTeam));
	}

	void RequestLeaveTeam()
	{
		m_LeavingTeam = true;
		RequestTeam(NO_TEAM, true);
	}

// ================================= TOGGLES =================================

	void ToggleCompass()
	{
		if (m_CompassUI)
			m_CompassUI.ToggleCompass();
	}

	void ToggleKOTH()
	{
		if (m_KOTHUI)
			m_KOTHUI.ToggleKOTH();
	}

	void ToggleSnD()
	{
		if (m_SnDUI)
			m_SnDUI.ToggleSnD();
	}
	
	override void Continue()
	{
		super.Continue();
		ShowEventWidgets(true);
	}

	override void ShowInventory()
	{
		ShowEventWidgets(false);
		super.ShowInventory();
	}

	override void HideInventory()
	{
		super.HideInventory();
		ShowEventWidgets(true);
	}

	override void OnPlayerRespawned(Man player)
	{
		super.OnPlayerRespawned(player);

		if (m_Mode == MODE_KOTH || m_Mode == MODE_SND)
			ShowEventWidgets(true);
	}

	void ShowEventWidgets(bool show)
	{
		if (m_Mode == MODE_KOTH && m_KOTHUI && m_CompassUI)
		{
			m_KOTHUI.ShowKOTH(show);
			m_CompassUI.ShowCompass(show);
		}

		if(m_Mode == MODE_SND && m_CompassUI)
		{
			m_SnDUI.ShowSnD(show);
			m_CompassUI.ShowCompass(show);
		}
	}

// ================================= UTILITY =============================================

	void SaveCartData(string cartData)
    {
        m_SavedCartData = cartData;
    }

    string LoadCartData()
    {
        return m_SavedCartData;
    }

	array<int> GetSavedLoadout()
	{
		return m_SavedLoadout;
	}

	void SetSavedLoadout(array<int> loadout)
	{
		if (!m_SavedLoadout)
			m_SavedLoadout = new array<int>();

		m_SavedLoadout.Copy(loadout);
	}

	bool CanLeaveTeam(PlayerIdentity identity)
	{
		if (!identity || m_LeavingTeam)
			return false;

		foreach (PlayerTeamData playerData : m_StoredPlayerList)
		{
			if (playerData.GetIdentity() == identity && playerData.GetTeam() != NO_TEAM)
			{
				return true;
			}
		}

		return false;
	}

	string GetChatChannelName() 
    {
        if (m_ChatChannelGlobal) 
		{
			return "Global"; 
		}
		else 
		{
			return "Direct"; 
		}
    }

	bool GetIsChatChannelGlobal()
    { 
		return m_ChatChannelGlobal; 
	}
}