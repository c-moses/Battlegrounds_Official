class KOTH_Base
{
    protected string m_CurrentLayer;
    ref array<ref BattlegroundsLocation> m_EventLocations;
    vector m_EventLocation;
    vector m_PreviousEventLocation;
    protected string m_CurrentLocationString;
    protected string m_CurrentCoordsString;
    protected Object m_LastGreenSmoke;
    protected Object m_LastRewardCrate;
    protected ref CurrencyHelper m_CurrencyHelper;
    protected ref BattlegroundsRewardManager m_RewardManager;
    protected ref array<Object> m_GlobalObjects = new array<Object>();
    protected BattlegroundsTeleporterPanel_V1 m_Teleporter;
    protected ref array<BattlegroundsTeleporterPanel_V1> m_Teleporters = new ref array<BattlegroundsTeleporterPanel_V1>();
    protected float m_CaptureProgress = 0.0;
    protected int m_NumPlayersInZone;
    ref SafezoneLocations m_Safezones;
    ref map<string, int> m_TeamCounts;
    string m_DominantTeam;

    // CONFIG
    float m_CaptureProgressSpeed;

// ==================================================================

    bool m_QueueFinish = false;
    void QueueFinish()
    {
        m_QueueFinish = true;
    }

    void CleanupLayer()
    {
        if (m_LastGreenSmoke)
        {
            GetGame().ObjectDelete(m_LastGreenSmoke);
            m_LastGreenSmoke = null;
        }
        if (m_LastRewardCrate)
        {
            GetGame().ObjectDelete(m_LastRewardCrate);
            m_LastRewardCrate = null;
        }

        foreach (Object teleporter : m_Teleporters)
        {
            if (teleporter) GetGame().ObjectDelete(teleporter);
        }
        m_Teleporters.Clear();

        foreach (Object obj : m_GlobalObjects)
        {
            if (obj) GetGame().ObjectDelete(obj);
        }
        
        m_GlobalObjects.Clear();
        m_CaptureProgress = 0.0;
        m_NumPlayersInZone = 0;
        m_DominantTeam = "";
        m_QueueFinish = false;
        m_PreviousEventLocation = m_EventLocation;
        m_EventLocation = vector.Zero;
    }

// ==================================================================

    void KOTH_Base() 
    {
        m_EventLocations = new array<ref BattlegroundsLocation>;
        m_CurrencyHelper = CurrencyHelper.GetInstance();
        m_RewardManager = BattlegroundsRewardManager.GetInstance();
        m_TeamCounts = new ref map<string, int>;
        m_DominantTeam = NO_TEAM;
    }

    void InitLayer(string currentLayer, float captureTime) 
    {
        m_CurrentLayer = currentLayer;
        m_CaptureProgressSpeed = 100.0 / captureTime;
        SpawnObjects();
    }

    void BGCreateNewEvent() {}

    void CreateSmokeEvent(vector centralLocation, string centralType, string peripheralType, float peripheralDistance)
    {
        m_EventLocation = centralLocation;
        CreateSmokeAtLocation(centralLocation, centralType);

        array<vector> directions = { 
            "0 0 1",      // North
            "0.707 0 0.707",   // NE
            "1 0 0",      // East
            "0.707 0 -0.707",  // SE
            "0 0 -1",     // South
            "-0.707 0 -0.707", // SW
            "-1 0 0",     // West
            "-0.707 0 0.707"   // NW
        };

        for (int i = 0; i < directions.Count(); i++)
        {
            vector smokePos = centralLocation + (directions[i] * peripheralDistance);
            smokePos = SnapToGround(smokePos);
            CreateSmokeAtLocation(smokePos, peripheralType);
        }
    }

    vector SnapToGround(vector pos)
    {
        float groundY = GetGame().SurfaceY(pos[0], pos[2]);
        pos[1] = groundY;
        return pos;
    }

    array<Man> GetPlayersInRadius(vector eventPosition, float radius, bool requireFullData = true)
    {
        array<Man> result = new array<Man>;
        array<Man> allPlayers = new array<Man>;
        GetGame().GetPlayers(allPlayers);

        float halfBoxSize = radius;
        float minX = eventPosition[0] - halfBoxSize;
        float maxX = eventPosition[0] + halfBoxSize;
        float minZ = eventPosition[2] - halfBoxSize;
        float maxZ = eventPosition[2] + halfBoxSize;
        float maxY = eventPosition[1] + halfBoxSize;
        float minY = eventPosition[1] - halfBoxSize;

        float radiusSquared = radius * radius;
        foreach (Man player: allPlayers)
        {
            vector playerPos = player.GetPosition();

            if (playerPos[0] >= minX && playerPos[0] <= maxX && playerPos[2] >= minZ && playerPos[2] <= maxZ && playerPos[1] >= minY && playerPos[1] <= maxY)
            {
                if (vector.DistanceSq(playerPos, eventPosition) <= radiusSquared)
                {
                    result.Insert(player);
                }
            }
        }

        return result;
    }

    void GetTeamsInRadius(array<Man> playersInRadius)
    {
        m_TeamCounts.Clear();
        foreach (Man player : playersInRadius)
        {
            PlayerBase playerBase = PlayerBase.Cast(player);
            if (playerBase && playerBase.IsAlive() && !playerBase.IsPlayerDisconnected())
            {
                string teamID = playerBase.GetArmband();

                if (m_TeamCounts.Contains(teamID))
                {
                    m_TeamCounts[teamID] = m_TeamCounts[teamID] + 1;
                }
                else
                {
                    m_TeamCounts.Insert(teamID, 1);
                }
            }
        }

        int maxCount = 0;
        ref array<string> tiedTeams = new array<string>;
        foreach (string team, int count : m_TeamCounts)
        {
            if (count > maxCount)
            {
                maxCount = count;
                tiedTeams.Clear();
                tiedTeams.Insert(team);
            }
            else if (count == maxCount)
            {
                tiedTeams.Insert(team);
            }
        }

        if (tiedTeams.Count() == 1)
        {
            m_DominantTeam = tiedTeams[0];
        }
        else
        {
            m_DominantTeam = NO_TEAM;
        }
    }

    void ProcessPlayersInZone(Man playerMan, int currencyAmount)
    {
        PlayerBase player = PlayerBase.Cast(playerMan);
        if (!player || !player.IsAlive() || player.IsPlayerDisconnected())
            return;

        PlayerIdentity playerID = player.GetIdentity();
        m_CurrencyHelper.UpdateCurrency(playerID.GetPlainId(), currencyAmount, playerID);
        m_RewardManager.UpdatePlayerStat(playerID, "cashEarned", currencyAmount);
    }
    
    void OnCaptureComplete()
    {
        if (m_LastRewardCrate)
        {
            GetGame().ObjectDelete(m_LastRewardCrate);
            m_LastRewardCrate = null;
        }

        m_LastRewardCrate = GetGame().CreateObject("BattlegroundsRewardCrate", m_EventLocation, false, true);

        if (m_LastRewardCrate)
        {
            EntityAI obj = EntityAI.Cast(m_LastRewardCrate);
            if (obj)
                V1Rewards.AddRewardLoot(obj);
        }
        else return;

        EntityAI fireworksEntity = EntityAI.Cast(GetGame().CreateObject("BGFireworks", m_EventLocation, false, true));
        
        if (fireworksEntity && fireworksEntity.IsKindOf("BGFireworks"))
        {
            BGFireworks fireworks = BGFireworks.Cast(fireworksEntity);
            if (fireworks) { fireworks.OnIgnitedThis(null); }
        }

        const float SEARCH_RADIUS = 300.0;

        array<Object> objects = new array<Object>;
        array<CargoBase> dummy2 = new array<CargoBase>;

        GetGame().GetObjectsAtPosition(m_EventLocation, SEARCH_RADIUS, objects, dummy2);

        for (int j = 0; j < objects.Count(); j++)
        {
            Object smoke = objects[j];
            if (smoke.IsKindOf("BGSmokeRed") || smoke.IsKindOf("BGSmokeGreen"))
            { GetGame().ObjectDelete(smoke); }
        }

        KOTHManager.GetInstance().ResetObjScore();
    }

    void SpawnObjects()
    {
        ref array<vector> teleporterPositions;
        ref array<vector> teleporterOrientations;
        string world = EventState.GetInstance().GetWorld();

        if (world == "chernarusplus")
        {
            teleporterPositions = {
                "15309.833984 213.008545 15312.430664", // SZ
                "10667.208008 57.732203 14383.083008", // West Novo
                "11535.500000 158.380997 10643.400391", // Berezino
                "11146.537109 134.609183 7044.333496", // Dolina
                "9151.910156 223.932999 3904.760010", // Pusta
                "6552.139160 390.487128 5619.106445", // Zub
                "3315.899902 185.869995 3943.340088", // Bor
                "1095.675171 149.833563 6631.342285", // Metalurg
                "2739.639893 273.933990 9998.120117", // Lopatino
                "588.039001 501.702460 13655.651367", // Tisy
                "5831.629883 226.561005 13562.599609",  // Polesovo
                "7391.143555 402.004331 9119.967773" // Dwarfs
            };

            teleporterOrientations = {
                "-150.857681 0.000000 -0.000000", // SZ
                "66.157791 -0.000000 -0.000000", // West Novo
                "-82.805374 0.000000 0.000000", // Berezino
                "163.935762 0.000000 0.000000", // Dolina
                "-18.245176 0.000000 -0.000000", // Pusta
                "-3.866974 0.000000 -0.000000", // Zub
                "-14.488347 0.000000 0.000000", // Bor
                "109.963234 0.000000 0.000000", // Metalurg
                "-93.350182 0.000000 -0.000000", // Lopatino
                "25.946198 0.000000 -0.000000", // Tisy
                "-119.845482 0.000000 -0.000000",  // Polesovo
                "-82.724953 0.000000 -0.000000" // Dwarfs
            };
        }
        else if (world == "enoch")
        {
            teleporterPositions = {
                "12719.130859 216.591095 12672.254883", // SZ
                "8433.601563 177.804077 12032.699219", // Kolembrody
                "3412.298828 252.745987 12224.300781", // Lukow
                "6403.489746 223.510925 8180.077148", // Nidek
                "11625.218750 188.312225 9602.833008", // Sitnik
                "833.808472 229.612457 8672.781250", // Lawenda
                "3309.943359 404.549866 5039.292969", // Sarnowek
                "7513.200195 287.052490 2548.830566", // Nadbor
                "11477.045898 329.690948 4550.761719" // Gieraltow
            };

            teleporterOrientations = {
                "30.710705 0.000000 -0.000000", // SZ
                "-124.019806 0.000000 -0.000000", // Kolembrody
                "-167.221893 0.000000 -0.000000", // Lukow
                "133.951508 0.000000 -0.000000", // Nidek
                "-149.753677 0.000000 -0.000000", // Sitnik
                "91.607002 0.000000 -0.000000", // Lawenda
                "45.108967 0.000000 -0.000000", // Sarnowek
                "49.630455 0.000000 -0.000000", // Nadbor
                "127.021919 0.000000 -0.000000" // Gieraltow
            };
        }
        else if (world == "sakhal")
        {
            teleporterPositions = {
                "11280.879883 29.014116 1801.231689", // SZ
                "5739.084961 3.211599 4474.886230", // Mili Island East
                "3993.408447 74.656326 6116.942871", // Mili Island Central
                "1393.185913 72.016663 7882.994141", // Mili Island West
                "6152.172363 36.028057 8630.403320", // Main Island SW
                "6365.448242 2.301039 12805.975586", // Main Island NW
                "9680.759766 352.467499 12143.720703", // Main Island Volcano
                "14294.735352 5.152500 11604.325195", // Main Island NE
                "12749.660156 7.512501 7319.935059", // Main Island SE
            };

            teleporterOrientations = {
                "-170.100296 0.000000 -0.000000", // SZ
                "0.636397 0.000000 -0.000000", // Mili Island East
                "-138.659302 0.000000 -0.000000", // Mili Island Central
                "-76.102638 0.000000 0.000000", // Mili Island West
                "-73.092476 0.000000 0.000000", // Main Island SW
                "-28.723324 0.000000 0.000000", // Main Island NW
                "-98.339355 -0.000000 -0.000000", // Main Island Volcano
                "9.245062 0.000000 -0.000000", // Main Island NE
                "61.922348 0.000000 -0.000000", // Main Island SE
            };
        }
        
        for (int i = 0; i < teleporterPositions.Count(); i++)
        {
            Object teleporter = SpawnTeleporter("BattlegroundsTeleporterPanel_V1", teleporterPositions[i], teleporterOrientations[i]);
            m_GlobalObjects.Insert(teleporter);
        }

        SetSafeZoneAndBuyAreas(teleporterPositions);
    }

    Object SpawnTeleporter(string teleporterClassname, vector position, vector orientation) 
    {
        BattlegroundsTeleporterPanel_V1 teleporter = BattlegroundsTeleporterPanel_V1.Cast(GetGame().CreateObject(teleporterClassname, position));
        teleporter.SetOrientation(orientation);
        m_Teleporters.Insert(teleporter);
        return teleporter;
    }

    void SetSafeZoneAndBuyAreas(ref array<vector> teleporterPositions)
    {
        EventState.GetInstance().SetMainSpawn(teleporterPositions[0]);
        KOTHManager.GetInstance().SetBuyZones(teleporterPositions);
        SafezoneLocations safezones = SafezoneLocations.GetInstance();
        safezones.ClearAllSafezones();
        for (int i = 0; i < teleporterPositions.Count(); i++)
        {
            int radius = 100;
            string zoneName = "Zone_" + i; // todo assign actual names
            vector safezonePos = teleporterPositions[i];
            safezonePos[1] = 0; // set Y value to 0

            safezones.AddSafezone(i, radius, safezonePos, zoneName);
        }
    }

    void CreateSmokeAtLocation(vector location, string type)
    {
        GetGame().CreateObject(type, location);
    }

    void SetCurrentLocationString(string locationName)
    {
        m_CurrentLocationString = locationName;
    }

    string GetCurrentLocationString()
    {
        return m_CurrentLocationString;
    }

    string GetDominantFaction()
    {
        return m_DominantTeam;
    }

    vector GetEventCoords()
    {
        return m_EventLocation;
    }

    float GetCaptureProgress()
    {
        return 0.0;
    }

    int GetNumPlayersInZone() 
    {
        return m_NumPlayersInZone;
    }

    bool StartsWith(string source, string prefix)
    {
        return source.IndexOf(prefix) == 0;
    }

    void SetTeleporterDestinations() {}
    void CheckCaptureProgress() {}
    void CheckCaptureRadius() {}
}

class BattlegroundsLocation
{
    vector m_Position;
    string m_Name;

    void BattlegroundsLocation(vector position, string name = "")
    {
        m_Position = position;
        m_Name = name;
    }

    vector GetPosition()
    {
        return m_Position;
    }

    string GetName()
    {
        return m_Name;
    }
}