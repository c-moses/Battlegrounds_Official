modded class InGameMenu extends UIScriptedMenu
{
	protected Widget 				m_Discord;
	protected Widget 				m_StatsPanel;
	protected TextWidget 			m_NameText;
	protected TextWidget 			m_ScoreText;
	protected TextWidget 			m_RankText;
	protected TextWidget 			m_XPLevel;
	protected TextWidget 			m_XPRequired;
	protected TextWidget 			m_PKillsText;
	protected TextWidget 			m_ZKillsText;
	protected TextWidget 			m_DeathsText;
	protected ProgressBarWidget 	m_RankProgress;
	protected Widget 				m_LeaveTeam;
	protected Widget 				m_VotingPanel;
	protected GridSpacerWidget 		m_VotingGrid;
	protected TextWidget 			m_LeaveTeamText;

	bool m_IsOnTeam = false;
	bool m_LeavingTeam = false;

	void ~InGameMenu()
	{
		HudShow(true);
		Mission mission = g_Game.GetMission();
		if (mission)
			mission.Continue();
	}

	override Widget Init()
	{
		Mission mission = g_Game.GetMission();
		if (mission)
			mission.Pause();

		MissionGameplay mgameplay = MissionGameplay.Cast(g_Game.GetMission());
		if (mgameplay)
		{
			mgameplay.ShowEventWidgets(false);
		}

		layoutRoot = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/IngameMenu.layout");
		m_ContinueButton			= layoutRoot.FindAnyWidget("continuebtn");
		m_SeparatorPanel			= layoutRoot.FindAnyWidget("separator_red");
		m_ExitButton				= layoutRoot.FindAnyWidget("exitbtn");
		m_RestartButton				= layoutRoot.FindAnyWidget("restartbtn");
		m_RespawnButton 			= layoutRoot.FindAnyWidget("respawn_button");
		m_RestartDeadRandomButton	= layoutRoot.FindAnyWidget("respawn_button_random");
		m_RestartDeadCustomButton	= layoutRoot.FindAnyWidget("respawn_button_custom");
		m_OptionsButton				= layoutRoot.FindAnyWidget("optionsbtn");
		m_HintPanel					= new UiHintPanel(layoutRoot.FindAnyWidget("hint_frame"));
		m_ServerInfoPanel 			= layoutRoot.FindAnyWidget("server_info");
		m_ServerIP 					= TextWidget.Cast(layoutRoot.FindAnyWidget("server_ip"));
		m_ServerPort 				= TextWidget.Cast(layoutRoot.FindAnyWidget("server_port"));
		m_ServerName 				= TextWidget.Cast(layoutRoot.FindAnyWidget("server_name"));
		m_FavoriteImage 			= layoutRoot.FindAnyWidget("favorite_image");
		m_UnfavoriteImage 			= layoutRoot.FindAnyWidget("unfavorite_image");
		m_CopyInfoButton 			= layoutRoot.FindAnyWidget("copy_button");
		m_Discord					= layoutRoot.FindAnyWidget("discordbtn");
		m_StatsPanel 				= layoutRoot.FindAnyWidget("StatsPanel");
		m_NameText 					= TextWidget.Cast(layoutRoot.FindAnyWidget("NameText"));
		m_ScoreText 				= TextWidget.Cast(layoutRoot.FindAnyWidget("ScoreText"));
		m_RankText 					= TextWidget.Cast(layoutRoot.FindAnyWidget("RankText"));
		m_XPLevel 					= TextWidget.Cast(layoutRoot.FindAnyWidget("XPLevel"));
		m_XPRequired 				= TextWidget.Cast(layoutRoot.FindAnyWidget("XPRequired"));
		m_PKillsText 				= TextWidget.Cast(layoutRoot.FindAnyWidget("PKillsText"));
		m_ZKillsText 				= TextWidget.Cast(layoutRoot.FindAnyWidget("ZKillsText"));
		m_DeathsText 				= TextWidget.Cast(layoutRoot.FindAnyWidget("DeathsText"));
		m_RankProgress 				= ProgressBarWidget.Cast(layoutRoot.FindAnyWidget("RankProgress"));
		m_LeaveTeam					= layoutRoot.FindAnyWidget("leaveteambtn");
		m_LeaveTeamText				= layoutRoot.FindAnyWidget("leaveteam_label");
		m_VotingPanel				= layoutRoot.FindAnyWidget("VotingPanel");
		m_VotingGrid				= GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("VotingGridSpacer"));
		
		if (GetGame().IsMultiplayer())
			ButtonSetText(m_RestartButton, "#main_menu_respawn");
		else ButtonSetText(m_RestartButton, "#main_menu_restart");
		
		HudShow(false);
		SetGameVersion();
		SetServerInfoVisibility(SetServerInfo() && g_Game.GetProfileOption(EDayZProfilesOptions.SERVERINFO_DISPLAY));
		SetBattlegroundsStats();
		
		return layoutRoot;
	}

	void SetBattlegroundsStats()
	{
		MissionGameplay msngameplay = MissionGameplay.Cast(g_Game.GetMission());
		if (msngameplay)
		{
			PlayerIdentity identity = PlayerIdentity.Cast(GetGame().GetPlayer().GetIdentity());
			if (identity)
			{
				string name = identity.GetName();
				if (msngameplay.CanLeaveTeam(identity))
				{
					m_LeaveTeam.Show(true);
				}

				/*
				if (msngameplay.CanVote())
				{
					m_VotingPanel.Show(true);
					//  todo : add vote layer options to m_VotingGrid
				}*/
			}

			m_NameText.SetText(name);
			m_PKillsText.SetText(FormatNumberWithCommas(msngameplay.m_StatPKills));
			m_ZKillsText.SetText(FormatNumberWithCommas(msngameplay.m_StatZKills));
			m_DeathsText.SetText(FormatNumberWithCommas(msngameplay.m_StatDeaths));
			m_ScoreText.SetText(FormatNumberWithCommas(msngameplay.m_StatScore) + " XP");
			m_RankText.SetText(msngameplay.m_StatLevel.ToString());

			LevelManager levelManager = new LevelManager();
			int currentLevel = msngameplay.m_StatLevel;
			int xpForCurrentLevel = levelManager.GetXPForLevel(currentLevel);
			int xpForNextLevel = levelManager.GetXPForNextLevel(currentLevel);
			int currentXP = msngameplay.m_StatScore;

			if (xpForCurrentLevel >= 0)
			{
				m_XPLevel.SetText(FormatNumberWithCommas(xpForCurrentLevel) + " XP");
			}

			if (xpForNextLevel > 0)
			{
				m_XPRequired.SetText(FormatNumberWithCommas(xpForNextLevel) + " XP");
			}
			else
			{
				m_XPRequired.SetText("MAX");
				m_RankProgress.SetCurrent(1);
				return;
			}

			int xpDelta = xpForNextLevel - xpForCurrentLevel;
			int progress = Math.Round(((float)(currentXP - xpForCurrentLevel) / xpDelta) * 100);
			progress = Math.Clamp(progress, 0, 100);
			m_RankProgress.SetCurrent(progress);
		}
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		// todo : if its a vote layer widget, check in mission gameplay that CanVote is true (may have changed since last check), if true, get the key string and print key string, and call mission gameplay SetVoting(false);

		if (w == m_LeaveTeam)
		{
			MissionGameplay mg = MissionGameplay.Cast(g_Game.GetMission());
			if (mg)
			{
				mg.RequestLeaveTeam();
				m_LeaveTeam.Show(false);
				m_LeavingTeam = true;
				NotificationSystem.AddNotificationExtended(NOTIFY_LENGTH, NOTIFY_LEAVE_TEAM);
				return true;
			}
		}

		if (w == m_Discord)
		{
			GetGame().OpenURL("https://discord.gg/aVahH8U4Hh");
			return true;
		}

		return super.OnClick(w, x, y, button);
	}

	override protected void OnClick_Respawn()
	{
		Man player = GetGame().GetPlayer();
		if (player && player.IsUnconscious() && !player.IsDamageDestroyed())
		{
			GetGame().GetUIManager().ShowDialog("#main_menu_respawn", "#main_menu_respawn_question", IDC_INT_RETRY, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
		}
		else
		{
			MissionGameplay missionGameplay = MissionGameplay.Cast(GetGame().GetMission());
			if (missionGameplay && missionGameplay.m_Mode == MODE_DAYZ)
			{
				GameRespawn(true);
			}
			else
			{
				if (GetGame().GetMission().GetRespawnModeClient() == GameConstants.RESPAWN_MODE_CUSTOM)
				{
					GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetUIManager().EnterScriptedMenu, MENU_RESPAWN_DIALOGUE, this);
				}
				else
				{
					GameRespawn(true);
				}
			}
		}
	}

	string FormatNumberWithCommas(int number)
	{
		string numberStr = number.ToString();
		string formatted = "";

		int count = 0;
		for (int i = numberStr.Length() - 1; i >= 0; i--)
		{
			formatted = numberStr[i] + formatted;
			count++;
			if (count % 3 == 0 && i != 0)
			{
				formatted = "," + formatted;
			}
		}

		return formatted;
	}
}