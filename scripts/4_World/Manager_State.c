class EventState
{
    protected bool m_IsInit = false;
    protected ref LayerConfig m_LayerConfig;
    protected ref array<string> m_VotableLayers;

    protected string m_World = "";
    protected string m_Layer = "";
    protected string m_Mode = "";
    protected string m_Version = "";

    protected int m_State;
    protected float m_StartTime;
    protected float m_Timer;

    protected vector m_MainSpawn;
    protected vector m_RedSpawn;
    protected vector m_BlueSpawn;
    protected bool m_SwitchingSides;
    protected int m_Reset = POS_STANDARD;

    protected bool m_RequiresRestart = false;

///////////////////////////////////

void EventTick()
{
    if (IsKOTH())
    {
        KOTHManager.GetInstance().EventTickKOTH();
    }

    if (IsSnD())
    {
        SnDManager.GetInstance().EventTickSnD(GetEventState());
    }
}

///////////////////////////////////

    void InitBattlegrounds()
    {
        LayerConfigLoader.CheckDirectory("$profile:Battlegrounds");
        if(!LayerConfigLoader.LoadConfig("$profile:Battlegrounds/Layer.json", m_LayerConfig))
        {
            LayerConfigLoader.CreateDefaultConfig("$profile:Battlegrounds/Layer.json");
            LayerConfigLoader.LoadConfig("$profile:Battlegrounds/Layer.json", m_LayerConfig);
        }
    }

///////////////////////////////////

    void NotifyAllPlayers(string messageType, string message)
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        for (int i = 0; i < players.Count(); i++)
        {
            PlayerBase player = PlayerBase.Cast(players[i]);
            if (player && player.GetIdentity())
            {
                PlayerIdentity identity = player.GetIdentity();
                NotificationSystem.SendNotificationToPlayerIdentityExtended(identity, NOTIFY_LENGTH, messageType, message);
            }
        }
    }

///////////////////////////////////

    void StartNewEvent(bool immediate)
    {
        LayerConfigLoader.LoadConfig("$profile:Battlegrounds/Layer.json", m_LayerConfig);
        string layerName = MapLayerConfig.GetLayerNameFromKey(m_LayerConfig.LayerMode);
        string newWorld = MapLayerConfig.GetWorldFromLayer(m_LayerConfig.LayerMode);

        if (immediate)
        {
            if (m_RequiresRestart)
            {
                ServerRPC.SendServerRPC("RPC_RESTART", "now");
                return;
            }
            else
            {
                m_Layer = m_LayerConfig.LayerMode;
                ServerRPC.SendServerRPC("RPC_LAYER", layerName);
                NotifyAllPlayers("New layer:", layerName);
            }
            
            m_Mode = LayerConfigLoader.GetMode(m_Layer);
            m_Version = LayerConfigLoader.GetVersion(m_Layer);

            if (m_Mode == MODE_KOTH)
                KOTHManager.GetInstance().Init(m_Layer, m_LayerConfig.RoundTime, m_LayerConfig.ScoreLimit);

            if (m_Mode == MODE_SND)
                SnDManager.GetInstance().Init(m_Layer, m_LayerConfig.RoundTime, m_LayerConfig.DefuseTime, m_LayerConfig.ScoreLimit, m_LayerConfig.PreRoundTime, m_LayerConfig.RoundSwitch);

            if (m_VotableLayers)
                m_VotableLayers.Clear();

            m_VotableLayers = MapLayerConfig.GetVotableLayers(m_World); // todo , send to players
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendSafezonesToPlayers, 1000, false);
        }
        else
        {
            PrepareNewEvent(false);
            if (m_Mode == MODE_DAYZ)
            {
                CleanupComplete(MODE_DAYZ);
                EventTeams.GetInstance().ResetMatchScores();
		        EventTeams.GetInstance().ResetTeams();
            }

            if (m_World != newWorld)
            {
                ServerRPC.SendServerRPC("RPC_RESTART", "queue");
                NotifyAllPlayers("Queued layer (requires restart):", layerName);
                m_RequiresRestart = true;
            }
            else
            {
                NotifyAllPlayers("Queued layer:", layerName);
            }
        }
    }

    void PrepareNewEvent(bool immediate)
    {
        if (m_Mode == MODE_KOTH)
        {
            KOTHManager.GetInstance().FinishEvent(immediate);
        }

        if(m_Mode == MODE_SND)
        {
            SnDManager.GetInstance().FinishEvent(immediate);
        }
    }

    void CleanupComplete(string mode)
    {
        if (mode == MODE_KOTH)
        {
            KOTHManager.GetInstance().ClearInstance();
        }

        if (mode == MODE_SND)
        {
            SnDManager.GetInstance().ClearInstance();
        }

        SetState(STATE_CLEANUP);
        SendEventStateToAllPlayers();
        StartNewEvent(true);
    }

    void SendSafezonesToPlayers()
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        if (m_Mode == MODE_KOTH)
        {
            SafezoneLocations safezones = SafezoneLocations.GetInstance();
            if (safezones)
            {
                foreach (Man kothplayer : players)
                {
                    PlayerIdentity kothidentity = kothplayer.GetIdentity();
                    if (kothidentity)
                    {
                        GetRPCManager().SendRPC("Battlegrounds", "SendSafezones", new Param1<SafezoneLocations>(safezones), true, kothidentity);
                    }
                }
            }
        }
        else if (m_Mode == MODE_SND || m_Mode == MODE_DAYZ)
        {
            foreach (Man sndplayer : players)
            {
                PlayerIdentity sndidentity = sndplayer.GetIdentity();
                if (sndidentity)
                {
                    GetRPCManager().SendRPC("Battlegrounds", "SendSafezones", new Param1<SafezoneLocations>(null), true, sndidentity);
                }
            }
        }
    }

///////////////////////////////////

    void SetTimer(float timer)
    {
        m_StartTime = GetGame().GetTime() / 1000.0;
        m_Timer = timer;
    }

    float GetEventTimer()
    {
        float currentTime = GetGame().GetTime() / 1000.0;
        float elapsedTime = currentTime - m_StartTime;
        return Math.Max(0, m_Timer - elapsedTime);
    }

///////////////////////////////////

    array<string> GetVotableLayers()
    {
        return m_VotableLayers;
    }

///////////////////////////////////

    string GetWorld()
    {
        return m_World;
    }

    string GetLayer()
    {
        return m_Layer;
    }

    string GetMode()
    {
        return m_Mode;
    }

    string GetVersion()
    {
        return m_Version;
    }

///////////////////////////////////

    bool IsDayZ()
    {
        return m_Mode == MODE_DAYZ;
    }

    bool IsKOTH()
    {
        return m_Mode == MODE_KOTH;
    }

    bool IsSnD()
    {
        return m_Mode == MODE_SND;
    }

///////////////////////////////////

    void SetState(int state)
    {
        m_State = state;
    }

    int GetEventState()
    {
        return m_State;
    }

///////////////////////////////////

    vector MainSpawn()
    {
        return m_MainSpawn;
    }

    vector BlueSpawn()
    {
        return m_BlueSpawn;
    }

    vector RedSpawn()
    {
        return m_RedSpawn;
    }

///////////////////////////////////

    void SetMainSpawn(vector mainspawn)
    {
        m_MainSpawn = mainspawn;
    }

    void SetBlueSpawn(vector blue)
    {
        m_BlueSpawn = blue;
    }

    void SetRedSpawn(vector red)
    {
        m_RedSpawn = red;
    }

///////////////////////////////////

    void SetIsSwitchingSides(bool switching)
    {
        m_SwitchingSides = switching;
        vector temp = m_BlueSpawn;
        m_BlueSpawn = m_RedSpawn;
        m_RedSpawn = temp;
    }

    bool IsSwitchingSides()
    {
        return m_SwitchingSides;
    }

///////////////////////////////////

    int GetResetPosFlag ()
    {
        return m_Reset;
    }

    void SetResetPosFlag(int resetFlag)
    {
        m_Reset = resetFlag;
    }

///////////////////////////////////

    void SendEventStateToPlayer(PlayerIdentity identity)
    {
        GetRPCManager().SendRPC("Battlegrounds", "ReceiveEventState", new Param3<string, int, float>(m_Mode, m_State, m_Timer), true, identity);
    }

    void SendEventStateToAllPlayers()
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        foreach (Man player : players)
        {
            PlayerIdentity identity = player.GetIdentity();
            if (identity)
            {
                GetRPCManager().SendRPC("Battlegrounds", "ReceiveEventState", new Param3<string, int, float>(m_Mode, m_State, m_Timer), true, identity);
            }
        }
    }

///////////////////////////////////

    void CleanupBattlegrounds()
    {
        CleanupKOTHItems();
        CleanupSnDItems();
    }

    void CleanupKOTHItems()
    {
        vector centerOfMap = Vector(7675, 0, 7675);
        float searchRadius = 10857;
        array<Object> foundItems = new array<Object>;
        GetGame().GetObjectsAtPosition(centerOfMap, searchRadius, foundItems, NULL);
        array<Object> toDelete = new array<Object>;

        if (foundItems && foundItems.Count() > 0)
        {
            foreach(Object obj : foundItems)
            {
                if (obj.IsInherited(BGSmokeRed) || obj.IsInherited(BGSmokeGreen) || obj.IsInherited(BattlegroundsRewardCrate) || obj.IsInherited(BGFireworks))
                    toDelete.Insert(obj);
            }
        }

        Print("[KOTH Cleanup] Deleting " + toDelete.Count() + " leftover event objects");

        foreach(Object delObj : toDelete)
        { 
            GetGame().ObjectDelete(delObj); 
        }

        foundItems.Clear();
        toDelete.Clear();
    }

    void CleanupSnDItems()
    {
        vector center = Vector(7680, 0, 7680);
        float radius = 7680;
        array<Object> foundObjects = new array<Object>;
        GetGame().GetObjectsAtPosition(center, radius, foundObjects, NULL);
        array<Object> toDelete = new array<Object>;

        if (foundObjects && foundObjects.Count() > 0)
        {
            foreach (Object obj : foundObjects)
            {
                if (obj.IsInherited(SnD_Bomb) || obj.IsInherited(SnD_Site) || obj.IsInherited(Offroad_SnD))
                    toDelete.Insert(obj);
            }
        }

        Print("[SnD Cleanup] Deleting " + toDelete.Count() + " leftover event objects");

        foreach (Object delObj : toDelete)
        {
            GetGame().ObjectDelete(delObj);
        }
        
        foundObjects.Clear();
        toDelete.Clear();
    }

///////////////////////////////////

    void Init(string world)
    {
        m_World = world;
        m_IsInit = true;
    }
    
    void EventState() {}
    static autoptr EventState s_Instance;
    static autoptr EventState GetInstance()
    {
        if (GetGame().IsServer())
        {
            if (!s_Instance)
            {
                s_Instance = new EventState();
            }

            return s_Instance;
        }
        else
        {
            return null;
        }
    }
}