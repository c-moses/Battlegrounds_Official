class EventTeams
{
    protected bool m_IsInit = false;
    private ref EventState m_EventState;
    private ref BattlegroundsRewardManager m_RewardManager;
	private ref CurrencyHelper m_CurrencyHelper;
    protected ref map<string, ref PlayerTeamData> m_PlayerTeams;
    protected ref map<string, int> m_TeamScores;
    protected int m_ScoreLimit;
    protected ref array<PlayerIdentity> m_SnDQueue = new array<PlayerIdentity>();
    protected ref array<string> m_LeaveQueue = new array<string>();
    protected ref map<string, string> m_PlayerVotes; // identity (string) and layer key (string)
    protected bool m_VotingActive = false;
    protected bool m_MatchEnded = false;

////////////////////////////////////////////////////////////

    void RequestLeaveTeam(string identity)
    {
        if (!identity || IsLeavingTeam(identity))
            return;

        m_LeaveQueue.Insert(identity);
    }

    bool IsLeavingTeam(string identity)
    {
        if (!identity)
            return false;

        return m_LeaveQueue.Find(identity) != -1;
    }

////////////////////////////////////////////////////////////

    void RewardSnDTeams(string winningTeam)
    {
        if (!m_PlayerTeams || winningTeam == "")
            return;

        const int WINNING_REWARD = 1500;
        const int LOSING_REWARD = 750;

        foreach (string steamID, PlayerTeamData playerData : m_PlayerTeams)
        {
            PlayerIdentity identity = playerData.GetIdentity();
            if (!identity)
                continue;

            int rewardAmount;
            if (playerData.GetTeam() == winningTeam)
                rewardAmount = WINNING_REWARD;
            else rewardAmount = LOSING_REWARD;

            m_CurrencyHelper.UpdateCurrency(identity.GetPlainId(), rewardAmount, identity);
            m_RewardManager.UpdatePlayerStat(identity, "cashEarned", rewardAmount);
        }
    }

////////////////////////////////////////////////////////////

    void ResetTeams()
    {
        foreach (string playerId, PlayerTeamData data : m_PlayerTeams)
        {
            data.SetTeam(NO_TEAM);
        }
        SendPlayerList();

        foreach (string pId, PlayerTeamData pdata : m_PlayerTeams)
        {
            if (pdata && pdata.GetIdentity())
            {
                PlayerBase player = PlayerHelper.GetPlayerByIdentity(pdata.GetIdentity());
                player.RemoveArmband();

                if (!m_EventState.IsDayZ())
                {
                    RequestSelection(pdata.GetIdentity(), REQUEST_TEAMSELECT);
                }
            }
        }
    }

    void SetPlayersAliveSnD()
    {
        foreach (PlayerIdentity identity : m_SnDQueue)
        {
            string playerId = identity.GetId();
            if (m_PlayerTeams.Contains(playerId))
            {
                PlayerTeamData playerData = m_PlayerTeams.Get(playerId);
                playerData.SetAlive(true);
                SendPlayerList();

                Print("[EventTeams] Player " + identity.GetName() + " is now marked alive for new SnD round.");
            }
        }

        m_SnDQueue.Clear();
    }

    void AddToSnDQueue(PlayerIdentity identity)
    {
        if (m_SnDQueue.Find(identity) == -1)
        {
            m_SnDQueue.Insert(identity);

            Print("[EventTeams] Player " + identity.GetName() + " added to SnD Queue");
        }
    }

////////////////////////////////////////////////////////////
    
    void SetPlayerAlive (PlayerIdentity identity, bool alive)
    {
        string playerId = identity.GetId();
        if (m_PlayerTeams.Contains(playerId))
        {
            PlayerTeamData playerData = m_PlayerTeams.Get(playerId);
            if (alive)
            {
                playerData.SetAlive(alive);
            }
            else
            {
                if (IsOnRedOrBlue(identity) && (m_EventState.GetEventState() == SND_STATE_ACTIVE || m_EventState.GetEventState() == SND_STATE_ARMED))
                {
                    if (IsLastAlive(identity))
                    {
                        string teamID = playerData.GetTeam();
                        SnDManager.GetInstance().TeamEliminated(teamID);
                    }

                    playerData.SetAlive(false);
                    AddToSnDQueue(identity);
                }
                else
                {
                    playerData.SetAlive(false);
                }
            }

            SendPlayerList();
        }
    }

    bool IsLastAlive(PlayerIdentity identity)
    {
        string playerId = identity.GetId();
        if (!m_PlayerTeams.Contains(playerId))
            return false;

        string teamID = m_PlayerTeams.Get(playerId).GetTeam();
        foreach (string id, PlayerTeamData playerData : m_PlayerTeams)
        {
            if (id != playerId && playerData.GetTeam() == teamID && playerData.IsAlive())
            {
                return false;
            }
        }

        return true;
    }

    string GetOpponents(string team)
    {
        if (team == TEAM_RED)
            return TEAM_BLUE;

        else if (team == TEAM_BLUE)
            return TEAM_RED;

        return "";
    }

////////////////////////////////////////////////////////////

    void RequestSelection(PlayerIdentity identity, int requestType)
    {
        GetRPCManager().SendRPC("Battlegrounds", "ReceiveSelectRequest", new Param1<int>(requestType), true, identity);
    }

    void SendPlayerList()
    {
        array<ref PlayerTeamData> playerList = new array<ref PlayerTeamData>;
        
        foreach (string playerId, PlayerTeamData data : m_PlayerTeams)
        {
            if (data && data.GetIdentity())
            {
                playerList.Insert(data);
            }
        }

        foreach (string pId, PlayerTeamData pdata : m_PlayerTeams)
        {
            if (pdata && pdata.GetIdentity())
            {
                PlayerIdentity identity = pdata.GetIdentity();
                GetRPCManager().SendRPC("Battlegrounds", "ReceivePlayerList", new Param1<array<ref PlayerTeamData>>(playerList), true, identity);
            }
        }
    }

    void SendEventScore()
    {
        ref array<ref Param2<string, int>> sortedScores = SortScoresDescending();
        foreach (string playerId, PlayerTeamData data : m_PlayerTeams)
        {
            if (data && data.GetIdentity())
            {
                PlayerIdentity identity = data.GetIdentity();
                GetRPCManager().SendRPC("Battlegrounds", "ReceiveScore", new Param1<array<ref Param2<string, int>>>(sortedScores), true, identity);
            }
        }
    }

    void SendEventScoreToPlayer(PlayerIdentity identity)
    {
        ref array<ref Param2<string, int>> sortedScores = SortScoresDescending();
        GetRPCManager().SendRPC("Battlegrounds", "ReceiveScore", new Param1<array<ref Param2<string, int>>>(sortedScores), true, identity);
    }


////////////////////////////////////////////////////////////

    void ResetMatchScores()
    {
        m_MatchEnded = false;
        m_TeamScores.Clear();
        if(m_EventState.IsSnD() || m_EventState.IsKOTH())
        {
            m_TeamScores.Set(TEAM_BLUE, 0);
            m_TeamScores.Set(TEAM_RED, 0);
            m_TeamScores.Set(TEAM_GREEN, 0);
            m_TeamScores.Set(TEAM_ORANGE, 0);
            m_TeamScores.Set(TEAM_YELLOW, 0);
            m_TeamScores.Set(TEAM_PINK, 0);
        }
        
        SendEventScore();
    }

    void UpdateScore(string teamID, int score)
    {
        if (m_MatchEnded)
            return;
        
        if (!PlayerHelper.IsEventTeam(teamID))
            return;

        if (m_TeamScores.Contains(teamID))
        {
            int currentScore = m_TeamScores.Get(teamID);
            m_TeamScores.Set(teamID, currentScore + score);
        }
        else
        {
            m_TeamScores.Set(teamID, score);
        }

        int highestScore = 0;
        foreach (string tID, int tScore : m_TeamScores)
        {
            if (tScore > highestScore)
            {
                highestScore = tScore;
            }
        }

        if (highestScore <= m_ScoreLimit - 2)
        {
            PlayerVoting(true);
        }
        else
        {
            PlayerVoting(false);
        }

        if (highestScore == m_ScoreLimit - 1)
        {
            string votedLayer = CalculateMostVotedLayer();
            if (votedLayer == "")
                votedLayer = m_EventState.GetLayer();

            LayerConfig layerCfg;
            LayerConfigLoader.LoadConfig("$profile:Battlegrounds/Layer.json", layerCfg);
            LayerConfigLoader.UpdateLayerConfig("$profile:Battlegrounds/Layer.json", votedLayer, layerCfg.ScoreLimit, layerCfg.RoundTime, layerCfg.DefuseTime, layerCfg.PreRoundTime, layerCfg.RoundSwitch);
            m_PlayerVotes.Clear();
        }

        SendEventScore();

        if (highestScore == m_ScoreLimit)
        {
            Print("END MATCH");
            m_MatchEnded = true;
            m_EventState.PrepareNewEvent(true);
            m_EventState.StartNewEvent(true);
        }
    }

////////////////////////////////////////////////////////////

    bool VotingIsActive()
    {
        return m_VotingActive;
    }

    bool HasVoted(string playerId)
    {
        return m_PlayerVotes.Contains(playerId);
    }

    void PlayerVoting(bool voting)
    {
        m_VotingActive = voting;
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        foreach (Man player : players)
        {
            PlayerBase pBase = PlayerBase.Cast(player);
            if (!pBase) continue;

            PlayerIdentity identity = pBase.GetIdentity();
            if (!identity) continue;

            string playerId = identity.GetId();
            
            if (voting)
            {
                if (!HasVoted(playerId))
                {
                    Print("[Event Teams] Requesting vote for identity: " + playerId);
                    RequestSelection(identity, REQUEST_VOTE);
                }
            }
            else
            {
                Print("[Event Teams] Requesting close vote for identity: " + playerId);
                RequestSelection(identity, CLOSE_VOTE);
            }
        }
    }

    protected string CalculateMostVotedLayer()
    {
        if (m_PlayerVotes.Count() == 0)
            return "";

        ref map<string, int> voteCounts = new map<string, int>();
        foreach (string identity, string layerKey : m_PlayerVotes)
        {
            if (voteCounts.Contains(layerKey))
            {
                voteCounts.Set(layerKey, voteCounts.Get(layerKey) + 1);
            }
            else
            {
                voteCounts.Set(layerKey, 1);
            }
        }

        string mostVotedLayer = "";
        int maxVotes = 0;
        foreach (string layerKy, int count : voteCounts)
        {
            if (count > maxVotes)
            {
                maxVotes = count;
                mostVotedLayer = layerKy;
            }
        }

        return mostVotedLayer;
    }

////////////////////////////////////////////////////////////

    bool EventStartConditions(int threshold)
    {
        int blueCount = 0;
        int redCount = 0;

        foreach (string playerId, PlayerTeamData data : m_PlayerTeams)
        {
            if (data)
            {
                string teamID = data.GetTeam();
                if (teamID == TEAM_BLUE)
                {
                    blueCount++;
                }
                else if (teamID == TEAM_RED)
                {
                    redCount++;
                }
            }
        }

        //return blueCount >= threshold || redCount >= threshold; // test server
        return blueCount >= threshold && redCount >= threshold; // online environment
    }

////////////////////////////////////////////////////////////

    array<PlayerIdentity> GetEventPlayers()
    {
        array<PlayerIdentity> teamPlayers = new array<PlayerIdentity>();
        foreach (string playerId, PlayerTeamData data : m_PlayerTeams)
        {
            if (data && data.GetIdentity())
                teamPlayers.Insert(data.GetIdentity());
        }
        return teamPlayers;
    }

    void UpdatePlayerTeam(PlayerIdentity identity, string teamID)
    {
        int index = m_LeaveQueue.Find(identity.GetId());
        if (index != -1)
        {
            m_LeaveQueue.Remove(index);
        }

        string playerId = identity.GetId();
        if (m_PlayerTeams.Contains(playerId))
        {
            PlayerTeamData existingData = m_PlayerTeams.Get(playerId);
            if (existingData.GetTeam() == teamID)
                return;

            existingData.SetTeam(teamID);
        }
        else
        {
            m_PlayerTeams.Set(playerId, new PlayerTeamData(identity, teamID));
        }

        Print("[EventTeams] UpdatePlayerTeam: " + identity.GetName() + " assigned to team " + teamID);
        SendPlayerList();

        if (teamID == NO_TEAM)
        {
            RequestSelection(identity, REQUEST_TEAMSELECT);
        }

        if (m_EventState.IsSnD() && m_EventState.GetEventState() == SND_STATE_INIT && EventStartConditions(2))
        {
            Print("[EventTeams] Search & Destroy start conditions met");
            SnDManager.GetInstance().ResetRound();
        }
    }

    void RemovePlayer(string userID)
    {
        if (m_PlayerTeams.Contains(userID))
        {
            m_PlayerTeams.Remove(userID);

            Print("[EventTeams] RemovePlayer: " + userID + " removed from team");
            SendPlayerList();
        }
    }

    bool IsPlayerOnTeam(PlayerIdentity identity)
    {
        return m_PlayerTeams.Contains(identity.GetId());
    }

    bool IsPlayerUnassigned(PlayerIdentity identity)
    {
        string playerId = identity.GetId();
        if (m_PlayerTeams.Contains(playerId))
        {
            PlayerTeamData playerData = m_PlayerTeams.Get(playerId);
            return playerData.GetTeam() == NO_TEAM;
        }
        return false;
    }

    bool IsOnRedOrBlue(PlayerIdentity identity)
    {
        string team = GetPlayerTeam(identity);
        return team == TEAM_BLUE || team == TEAM_RED;
    }

    string GetPlayerTeam(PlayerIdentity identity)
    {
        if (m_PlayerTeams.Contains(identity.GetId()))
        {
            return m_PlayerTeams.Get(identity.GetId()).GetTeam();
        }
        return "";
    }

    ref array<ref Param2<string, int>> SortScoresDescending()
    {
        ref array<ref Param2<string, int>> sortedScores = new array<ref Param2<string, int>>();
        foreach (string teamID, int score : m_TeamScores)
        {
            sortedScores.Insert(new Param2<string, int>(teamID, score));
        }

        for (int i = 0; i < sortedScores.Count() - 1; i++)
        {
            for (int j = 0; j < sortedScores.Count() - i - 1; j++)
            {
                if (sortedScores[j].param2 < sortedScores[j + 1].param2)
                {
                    Param2<string, int> temp = sortedScores[j];
                    sortedScores[j] = sortedScores[j + 1];
                    sortedScores[j + 1] = temp;
                }
            }
        }
        return sortedScores;
    }

///////////////////////////////////

    void SetScoreLimit(int limit)
    {
        m_ScoreLimit = limit;
    }
    
    void Init()
    {
        m_EventState = EventState.GetInstance();
        m_RewardManager = BattlegroundsRewardManager.GetInstance();
        m_CurrencyHelper = CurrencyHelper.GetInstance();
        m_PlayerTeams = new map<string, ref PlayerTeamData>();
        m_TeamScores = new map<string, int>();
        m_PlayerVotes = new map<string, string>();
        m_IsInit = true;
    }
    
    void EventTeams() {}
    static autoptr EventTeams s_Instance;
    static autoptr EventTeams GetInstance()
    {
        if (GetGame().IsServer())
        {
            if (!s_Instance)
            {
                s_Instance = new EventTeams();
            }

            return s_Instance;
        }
        else
        {
            return null;
        }
    }
}

///////////////////////////////////

class PlayerTeamData
{
    PlayerIdentity identity;
    string team;
    bool isAlive;

    void PlayerTeamData(PlayerIdentity pidentity, string pteam)
    {
        this.identity = pidentity;
        this.team = pteam;
        this.isAlive = true;
    }

    PlayerIdentity GetIdentity()
    {
        return identity;
    }

    string GetTeam()
    {
        return team;
    }

    void SetTeam(string newTeam)
    {
        team = newTeam;
    }

    bool IsAlive()
    {
        return isAlive;
    }

    void SetAlive(bool alive)
    {
        isAlive = alive;
    }
}