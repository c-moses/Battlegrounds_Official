class SnDManager
{
    ref EventState m_EventState;
    ref EventTeams m_EventTeams;

    protected ref SnD_Base m_Map;
    protected int m_initSwitchLimit;
    protected float m_initPreRoundDuration;
    protected float m_initRoundDuration;
    protected float m_initBombDuration;

    protected bool m_IsInit = false;
    protected bool m_MatchStarted = false;
    protected float m_PreRoundDuration = 0;
    protected float m_RoundDuration = 0;
    protected float m_BombDuration = 0;
    protected int m_CurrentRound = 0;
    protected int m_UntilSwitch;

    protected vector m_SiteA;
    protected vector m_SiteB;
    protected string m_ArmedBy;
    protected int m_ArmedSite = 0;

    protected bool m_QueueFinish = false;

/////////////////// SERVER > CLIENT ///////////////////

	void SendEventDataToPlayer(PlayerIdentity identity)
    {
        GetRPCManager().SendRPC("Battlegrounds", "ReceiveSnD", new Param5<int, int, vector, vector, int>(m_CurrentRound, m_UntilSwitch, m_SiteA, m_SiteB, m_ArmedSite), true, identity);
        m_EventTeams.SendEventScoreToPlayer(identity);
    }

    void SendEventDataToPlayers()
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        foreach (Man player : players)
        {
            PlayerIdentity identity = player.GetIdentity();
            if (identity)
            {
                GetRPCManager().SendRPC("Battlegrounds", "ReceiveSnD", new Param5<int, int, vector, vector, int>(m_CurrentRound, m_UntilSwitch, m_SiteA, m_SiteB, m_ArmedSite), true, identity);
            }
        }
    }

    void NotifyPlayers(string team, string msgType, string icon = "")
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        string message = GetTeamName(team) + msgType;
        foreach (Man player : players)
        {
            PlayerIdentity identity = player.GetIdentity();
            if (identity)
            {
                NotificationSystem.SendNotificationToPlayerIdentityExtended(identity, NOTIFY_LENGTH, message, "", icon);
            }
        }
    }

/////////////////// SnD STATES ///////////////////

    void StartGame()
    {
        m_EventState.SetState(SND_STATE_INIT);
        m_EventState.SetTimer(0);
        m_EventState.SendEventStateToAllPlayers();
        m_EventTeams.ResetMatchScores();
        m_EventTeams.ResetTeams();
        m_UntilSwitch = m_initSwitchLimit;
        SendEventDataToPlayers();
    }

    void ResetRound()
    {
        m_ArmedBy = "";
        m_ArmedSite = 0;
        m_CurrentRound++;
        HandleSideSwitching();
        m_PreRoundDuration = m_initPreRoundDuration;
        m_RoundDuration = m_initRoundDuration;
        m_BombDuration = m_initBombDuration;
        m_EventState.SetState(SND_STATE_ACTIVE_BUY);
        m_EventState.SetTimer(m_RoundDuration);
        m_EventState.SendEventStateToAllPlayers();
        m_Map.ResetSnDRound();
        ResetPlayers();
        m_EventTeams.SetPlayersAliveSnD();
        m_MatchStarted = true;
        SendEventDataToPlayers();
    }

    void LockRound()
    {
        m_EventState.SetState(SND_STATE_ACTIVE);
        m_EventState.SendEventStateToAllPlayers();
        SendEventDataToPlayers();
    }

    void EndGame()
    {
        m_CurrentRound = 0;
        m_EventState.SetState(SND_STATE_COMPLETED);
        m_EventState.SetTimer(0);
        m_EventState.SendEventStateToAllPlayers();
        m_Map.EndSnDGame();
        m_MatchStarted = false;
        SendEventDataToPlayers();
        // after action report
    }

/////////////////// EVENTS ///////////////////

    void BombArmed(string team, string site)
    {
        m_EventState.SetState(SND_STATE_ARMED);
        m_EventState.SetTimer(m_BombDuration);
        m_EventState.SendEventStateToAllPlayers();
        m_ArmedBy = team;

        if (site == "SnD_SiteA")
            m_ArmedSite = 1;

        if (site == "SnD_SiteB")
            m_ArmedSite = 2;

        NotifyPlayers(team, NOTIFY_SND_ARMED, ICON_SND_ARMED);
        SendEventDataToPlayers();
    }

    void BombDisarmed(string team)
    {
        ProcessScore(team);
        NotifyPlayers(team, NOTIFY_SND_DISARMED, ICON_SND_DISARMED);
    }

    void TeamEliminated(string team)
    {
        if (m_EventState.GetEventState() == SND_STATE_ARMED)
        {
            if (m_ArmedBy == team)
                return;
        }

        ProcessScore(GetOpponents(team));
        NotifyPlayers(team, NOTIFY_SND_ELIMINATED, ICON_SND_ELIMINATED);
    }

    void BombDisconnected()
    {
        Print("SnD: Bomb disconnected");
    }

/////////////////// SnD CORE ///////////////////

    void ProcessScore(string winningTeam)
    {
        m_EventTeams.UpdateScore(winningTeam, 1);

        if (m_QueueFinish)
        {
            m_Map.CleanupLayer();
            m_EventState.CleanupComplete(MODE_SND);
            return;
        }

        if (m_EventState.GetEventState() != SND_STATE_COMPLETED)
        {
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ResetRound, 5000, false);
            m_EventTeams.RewardSnDTeams(winningTeam);
        }
        else
        {
            // end game
        }
    }

    void ResetPlayers()
    {
        array<PlayerIdentity> teamPlayers = m_EventTeams.GetEventPlayers();
        foreach (PlayerIdentity identity : teamPlayers)
        {
            if (identity)
            {
                if (m_EventTeams.GetPlayerTeam(identity) == TEAM_BLUE)
                    PlayerHelper.ResetToPos(identity, m_EventState.BlueSpawn(), 1.0, 5.0, m_EventState.GetResetPosFlag(), true);

                if (m_EventTeams.GetPlayerTeam(identity) == TEAM_RED)
                    PlayerHelper.ResetToPos(identity, m_EventState.RedSpawn(), 1.0, 5.0, m_EventState.GetResetPosFlag(), true);
            }
        }
    }

/////////////////// TIMERS ///////////////////

    void EventTickSnD(int eventState)
    {
        if (!GetGame() || !m_IsInit)
            return;

        if (eventState == SND_STATE_ACTIVE || eventState == SND_STATE_ACTIVE_BUY)
        {
            HandleRoundTime(eventState);
        }

        if (eventState == SND_STATE_ARMED)
        {
            HandleBombTime();
        }
    }

    void HandleRoundTime(int eventState)
    {
        if (eventState == SND_STATE_ACTIVE_BUY)
        {
            if (m_PreRoundDuration > 0)
            {
                m_PreRoundDuration -= 1.0;
                if (m_PreRoundDuration <= 0)
                {
                    LockRound();
                }
            }
        }

        if (m_RoundDuration > 0)
        {
            m_RoundDuration -= 1.0;
            m_EventState.SetTimer(m_RoundDuration);
            
            if (m_RoundDuration <= 0)
            {
                if (!m_EventState.IsSwitchingSides())
                {
                    ProcessScore(TEAM_BLUE);
                    NotifyPlayers(TEAM_BLUE, NOTIFY_SND_TIME_LIMIT, ICON_SND_TIME_LIMIT);
                }
                else
                {
                    ProcessScore(TEAM_RED);
                    NotifyPlayers(TEAM_RED, NOTIFY_SND_TIME_LIMIT, ICON_SND_TIME_LIMIT);
                }
            }
        }
    }

    void HandleBombTime()
    {
        if (m_BombDuration > 0)
        {
            m_BombDuration -= 1.0;
            m_EventState.SetTimer(m_BombDuration);

            if (m_BombDuration <= 0)
            {
                if (m_ArmedBy == TEAM_BLUE || m_ArmedBy == TEAM_RED)
                {
                    m_Map.DetonateBomb();
                    ProcessScore(m_ArmedBy);
                    NotifyPlayers(m_ArmedBy, NOTIFY_SND_DETONATED, ICON_SND_DETONATED);
                }
                else // edge case
                {
                    GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ResetRound, 5000, false);
                }
            }
        }
    }

    void HandleSideSwitching()
    {
        if (m_MatchStarted)
            m_UntilSwitch--;

        if (m_UntilSwitch <= 0)
        {
            bool currentSwitchingState = m_EventState.IsSwitchingSides();
            m_EventState.SetIsSwitchingSides(!currentSwitchingState);
            m_UntilSwitch = m_initSwitchLimit;
            m_Map.CleanSpawnAreas();
            NotifyPlayers("", NOTIFY_SND_SWITCH, ICON_SND_SWITCH);
        }
    }

    string GetOpponents(string team)
    {
        if (team == TEAM_RED)
            return TEAM_BLUE;

        else if (team == TEAM_BLUE)
            return TEAM_RED;

        return "";
    }

    string GetTeamName(string team)
    {
        if (team == TEAM_RED)
            return "Red team";

        else if (team == TEAM_BLUE)
            return "Blue team";

        return "";
    }

    void SetBombSitePos(vector siteA, vector siteB)
    {
        m_SiteA = siteA;
        m_SiteB = siteB;
    }

/////////////////// INIT MAP ///////////////////

    void Init(string sndmap, float round, float defuse, int scoreLimit, float preRound, int rSwitch)
    {
        switch(sndmap)
        {
            case "snd_101":
                m_Map = new snd_101();
                break;

            case "snd_102":
                m_Map = new snd_102();
                break;

            case "snd_103":
                m_Map = new snd_103();
                break;

            case "snd_104":
                m_Map = new snd_104();
                break;

            case "snd_105":
                m_Map = new snd_105();
                break;

            case "snd_106":
                m_Map = new snd_106();
                break;

            case "snd_107":
                m_Map = new snd_107();
                break;

            case "snd_201":
                m_Map = new snd_201();
                break;

            case "snd_202":
                m_Map = new snd_202();
                break;

            case "snd_203":
                m_Map = new snd_203();
                break;

            case "snd_204":
                m_Map = new snd_204();
                break;

            case "snd_205":
                m_Map = new snd_205();
                break;

            case "snd_206":
                m_Map = new snd_206();
                break;

            case "snd_207":
                m_Map = new snd_207();
                break;

            case "snd_208":
                m_Map = new snd_208();
                break;

            case "snd_209":
                m_Map = new snd_209();
                break;

            case "snd_210":
                m_Map = new snd_210();
                break;

            default:
				m_Map = new snd_106();
                break;
            
        }

        if (m_Map)
        {
            m_Map.InitMap(sndmap);
        }

        m_initRoundDuration = round;
        m_initPreRoundDuration = preRound;
        m_initBombDuration = defuse;
        m_initSwitchLimit = rSwitch;

        m_EventTeams.ResetMatchScores();
		m_EventTeams.ResetTeams();
        m_EventTeams.SetScoreLimit(scoreLimit);

        StartGame();
        m_IsInit = true;
    }

/////////////////// INSTANCE ///////////////////

    void SnDManager() 
    {
        m_EventState = EventState.GetInstance();
        m_EventTeams = EventTeams.GetInstance();
    }

    static autoptr SnDManager m_Instance;
    static autoptr SnDManager GetInstance()
    {
        if (GetGame().IsServer())
		{
			if (!m_Instance)
			{
				m_Instance = new SnDManager();
			}

			return m_Instance;
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
            m_QueueFinish = true;
        }
		else
		{
			m_Map.CleanupLayer();
        	m_Map = null;
		}
	}

    void ClearInstance()
    {
        m_IsInit = false;
        m_initSwitchLimit = 0;
        m_initPreRoundDuration = 0.0;
        m_initRoundDuration = 0.0;
        m_initBombDuration = 0.0;
        m_MatchStarted = false;
        m_PreRoundDuration = 0;
        m_RoundDuration = 0;
        m_BombDuration = 0;
        m_CurrentRound = 0;
        m_UntilSwitch = 0;
        m_SiteA = vector.Zero;
        m_SiteB = vector.Zero;
        m_ArmedBy = "";
        m_ArmedSite = 0;
        m_QueueFinish = false;
        m_Instance = null;
    }
}

    /*void DebugPrintTimers()
    {
        float eventTimer = m_EventState.GetEventTimer();
        Print("[DEBUG] Round Timer: " + m_RoundDuration.ToString() + " | Bomb Timer: " + m_BombDuration.ToString() + " | EventState Timer: " + eventTimer.ToString());
    }*/