class KOTHManager
{
    protected bool m_Inited = false;
	protected ref KOTH_Base m_CurrentLayer;
	protected int m_TickCount = 0;
	protected int m_ObjTick = 10;

	ref BattlegroundsRewardManager m_RewardManager;
	ref CurrencyHelper m_CurrencyHelper;
	ref EventState m_EventState;
	ref EventTeams m_EventTeams;

	private string m_PreviousLocationName;
	private float m_PreviousCaptureProgress;
	private int m_PreviousPlayersInZoneCount;
	private string m_PreviousDominantFaction;
	private vector m_PreviousLocationCoords;

	vector m_StoredEventCoords;
	bool dataChanged = true;

	protected ref map<string, int> m_ObjScore = new map<string, int>();

	ref array<vector> m_BuyZoneVectors;
	void SetBuyZones(ref array<vector> buyZones)
	{
		m_BuyZoneVectors = buyZones;
	}

// ================================= SCORING =============================================

	void AddSecondsHeld(string teamId, int secHeld)
	{
		if (teamId == string.Empty)
			return;

		if (!m_ObjScore.Contains(teamId))
		{
			m_ObjScore[teamId] = secHeld;
		}
		else
		{
			int currentSeconds = m_ObjScore.Get(teamId);
			m_ObjScore[teamId] = currentSeconds + secHeld;
		}
	}

	void ResetObjScore()
    {
        string topTeamId = "";
        int maxSeconds = 0;

        foreach (string teamId, int secHeld : m_ObjScore)
        {
            Print(string.Format("Team: %1, Seconds Held: %2", teamId, secHeld));

            if (secHeld > maxSeconds)
            {
                maxSeconds = secHeld;
                topTeamId = teamId;
            }
        }

        if (topTeamId != "")
        {
            m_EventTeams.UpdateScore(topTeamId, 1);
            Print(string.Format("Team %1 awarded 1 point for holding the objective", topTeamId));
        }

        m_ObjScore.Clear();
    }

// ================================= EVENT TICK =============================================

	void EventTickKOTH()
	{
		if (!GetGame() || !m_Inited)
			return;

		if (m_CurrentLayer)
		{
			m_CurrentLayer.CheckCaptureProgress();
			GetEventData();
		}

		m_TickCount++;
		if (m_TickCount >= m_ObjTick)
		{
			m_TickCount = 0;
			if (m_CurrentLayer)
			{
				m_CurrentLayer.CheckCaptureRadius();
				if (PlayerHelper.IsEventTeam(m_PreviousDominantFaction))
				{
					AddSecondsHeld(m_PreviousDominantFaction, m_ObjTick);
				}
			}
		}
	}

// ================================= SERVER > CLIENT =============================================

	void GetEventData()
	{
		if (m_CurrentLayer)
		{
			string newLocationName = m_CurrentLayer.GetCurrentLocationString();
			vector newLocationCoords = m_CurrentLayer.GetEventCoords();
			float newCaptureProgress = m_CurrentLayer.GetCaptureProgress();
			int newPlayersInZoneCount = m_CurrentLayer.GetNumPlayersInZone();
			string newDominantFaction = m_CurrentLayer.GetDominantFaction();

			if (newLocationName == m_PreviousLocationName && newLocationCoords == m_PreviousLocationCoords && newCaptureProgress == m_PreviousCaptureProgress && newPlayersInZoneCount == m_PreviousPlayersInZoneCount && newDominantFaction == m_PreviousDominantFaction)
			{
				dataChanged = false;
			}
			else
			{
				dataChanged = true;
			}

			m_PreviousLocationName = newLocationName;
			m_PreviousLocationCoords = newLocationCoords;
			m_PreviousCaptureProgress = newCaptureProgress;
			m_PreviousPlayersInZoneCount = newPlayersInZoneCount;
			m_PreviousDominantFaction = newDominantFaction;

			if (dataChanged)
			{
				SendEventData();
			}
		}
	}

	void SendEventData() // to all players
	{
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		foreach(Man player: players)
		{
			PlayerBase pb = PlayerBase.Cast(player);
			if (pb)
			{
				ref Param5<string, float, int, string, vector> eventData = new Param5<string, float, int, string, vector>(m_PreviousLocationName, m_PreviousCaptureProgress, m_PreviousPlayersInZoneCount, m_PreviousDominantFaction, m_PreviousLocationCoords);
				GetRPCManager().SendRPC("Battlegrounds", "ReceiveKOTH", eventData, true, pb.GetIdentity());
			}
		}
	}

	void SendEventDataToPlayer(PlayerIdentity identity) // to specific player
    {
        ref Param5<string, float, int, string, vector> eventData = new Param5<string, float, int, string, vector>(m_PreviousLocationName, m_PreviousCaptureProgress, m_PreviousPlayersInZoneCount, m_PreviousDominantFaction, m_PreviousLocationCoords);
        GetRPCManager().SendRPC("Battlegrounds", "ReceiveKOTH", eventData, true, identity);
		GetRPCManager().SendRPC("Battlegrounds", "ReceiveBuyZones", new Param1<array<vector>>(m_BuyZoneVectors), true, identity); // todo move to sz module? 
		m_EventTeams.SendEventScoreToPlayer(identity);
    }

	void SendBuyZonesToAllPlayers()
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        foreach (Man player : players)
        {
            PlayerIdentity identity = player.GetIdentity();
            if (identity)
            {
                GetRPCManager().SendRPC("Battlegrounds", "ReceiveBuyZones", new Param1<array<vector>>(m_BuyZoneVectors), true, identity);
            }
        }
    }

// ================================= INIT =============================================

    void Init(string layerMode, float captureTime, int scoreLimit)
    {
        switch(layerMode)
        {
            case "koth_101":
                m_CurrentLayer = new koth_101();
                break;

			case "koth_102":
				m_CurrentLayer = new koth_102();
				break;

			case "koth_103":
				m_CurrentLayer = new koth_103();
				break;

			case "koth_104":
				m_CurrentLayer = new koth_104();
				break;

			case "koth_105":
				m_CurrentLayer = new koth_105();
				break;

			case "koth_106":
				m_CurrentLayer = new koth_106();
				break;

			case "koth_107":
				m_CurrentLayer = new koth_107();
				break;

			case "koth_108":
				m_CurrentLayer = new koth_108();
				break;

			case "koth_109":
				m_CurrentLayer = new koth_109();
				break;

			case "koth_110":
				m_CurrentLayer = new koth_110();
				break;

			case "koth_111":
				m_CurrentLayer = new koth_111();
				break;

			case "koth_112":
				m_CurrentLayer = new koth_112();
				break;

			case "koth_113":
				m_CurrentLayer = new koth_113();
				break;

			case "koth_114":
				m_CurrentLayer = new koth_114();
				break;

			case "koth_201":
				m_CurrentLayer = new koth_201();
				break;

			case "koth_202":
				m_CurrentLayer = new koth_202();
				break;

			case "koth_203":
				m_CurrentLayer = new koth_203();
				break;

			case "koth_204":
				m_CurrentLayer = new koth_204();
				break;

			case "koth_205":
				m_CurrentLayer = new koth_205();
				break;

			case "koth_206":
				m_CurrentLayer = new koth_206();
				break;

			case "koth_301":
				m_CurrentLayer = new koth_301();
				break;

			default:
				m_CurrentLayer = new koth_101();
                break;

        }

        if(m_CurrentLayer)
        {
            m_CurrentLayer.InitLayer(layerMode, captureTime);
        }

		m_EventState.SetState(KOTH_STATE_INIT);
		m_EventState.SendEventStateToAllPlayers();
		m_EventTeams.ResetMatchScores();
		m_EventTeams.ResetTeams();
		m_EventTeams.SetScoreLimit(scoreLimit);
		SendBuyZonesToAllPlayers();
        m_Inited = true;
    }

// ================================= INSTANCE =============================================

	void KOTHManager()
    {
		m_RewardManager = BattlegroundsRewardManager.GetInstance();
		m_CurrencyHelper = CurrencyHelper.GetInstance();
		m_EventState = EventState.GetInstance();
		m_EventTeams = EventTeams.GetInstance();
    }

    static autoptr KOTHManager g_Instance;
	static autoptr KOTHManager GetInstance()
	{
		if (GetGame().IsServer())
		{
			if (!g_Instance)
				g_Instance = new KOTHManager();

			return g_Instance;
		}
		else
		{
			return null;
		}
	}

	void FinishEvent(bool immediate)
	{
		if (!immediate)
        {
            m_CurrentLayer.QueueFinish();
        }
		else
		{
			m_CurrentLayer.CleanupLayer();
        	m_CurrentLayer = null;
		}
	}

	static void ClearInstance()
    {
        if (g_Instance)
        {
            g_Instance.m_Inited = false;
			g_Instance.m_TickCount = 0;
            g_Instance.dataChanged = false;
            g_Instance.m_BuyZoneVectors.Clear();
            g_Instance.m_StoredEventCoords = vector.Zero;
            g_Instance.m_PreviousLocationName = "";
            g_Instance.m_PreviousCaptureProgress = 0.0;
            g_Instance.m_PreviousPlayersInZoneCount = 0;
            g_Instance.m_PreviousDominantFaction = "";
            g_Instance.m_PreviousLocationCoords = vector.Zero;
            g_Instance = null;
        }
    }
}