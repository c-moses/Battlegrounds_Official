class koth_201 extends KOTH_Base // Battle of Livonia
{
    vector m_EventPosition;
	vector m_PreviousEventPosition;
    bool m_PreviousEventPositionSet;
    string m_CurrentLocationName, m_LastLocationName, m_SecondLastLocationName;
    protected ref array<Object> m_CurrentObjects = new array<Object> ();
	private bool m_LastCheckedPlayerInZone = false;
    const float CAPTURE_PROGRESS_CHECK_INTERVAL = 1.0;
    const float PLAYERS_IN_RADIUS_CHECK_INTERVAL = 10.0;
	int m_CurrentLocationIndex = -1;
    int m_CycleDirection = 1;

    override void InitLayer(string currentLayer, float captureTime) 
    {
        super.InitLayer(currentLayer, captureTime);

		InitLocations();
        if (Math.RandomIntInclusive(-1, 1) == 1)
        {
            m_CycleDirection = 1;  // Clockwise
        }
        else
        {
            m_CycleDirection = -1;  // Counterclockwise
        }
        BGCreateNewEvent();
    }

    override void BGCreateNewEvent()
    {
        if (m_QueueFinish)
        {
            CleanupLayer();
            EventState.GetInstance().CleanupComplete(MODE_KOTH);
            return;
        }
        
        BattlegroundsLocation location = GetNextLocation();
        if (location)
        {
            m_SecondLastLocationName = m_LastLocationName;
            m_LastLocationName = m_CurrentLocationName;

            m_EventPosition = location.GetPosition();
            SetCurrentLocationString(location.GetName());
            CreateSmokeEvent(m_EventPosition, "BGSmokeGreen", "BGSmokeRed", 100.0);
                
            m_CaptureProgress = 0.0;
            m_PreviousEventPosition = m_EventPosition;
            m_PreviousEventPositionSet = true;

            Print("[Battlegrounds v201] New Event: " + GetCurrentLocationName());
        }

        SetTeleporterDestinations();
    }

    override void SetTeleporterDestinations() 
    {
        if (m_Teleporters && m_Teleporters.Count() > 0)
        {
            vector destination = m_EventPosition;
            for (int i = 0; i < m_Teleporters.Count(); i++)
            {
                m_Teleporters[i].SetDestination(destination);
            }
        }
    }

	override void CheckCaptureProgress()
    {
        int numPlayersInZone = GetCEApi().CountPlayersWithinRange(m_EventLocation, 150);

        m_NumPlayersInZone = numPlayersInZone;

        if (m_CaptureProgress >= 100.0 || (numPlayersInZone == 0 && m_CaptureProgress <= 0.0))
        {
            return;
        }

        if (numPlayersInZone > 0)
        {
            m_CaptureProgress += m_CaptureProgressSpeed * CAPTURE_PROGRESS_CHECK_INTERVAL;
        }
        else
        {
            m_CaptureProgress -= m_CaptureProgressSpeed * CAPTURE_PROGRESS_CHECK_INTERVAL;
        }

        m_CaptureProgress = Math.Clamp(m_CaptureProgress, 0.0, 100.0);

        if (m_CaptureProgress == 100.0)
        {
            OnCaptureComplete();
            BGCreateNewEvent();
        } 
    }

    override void CheckCaptureRadius()
    {
        array<Man> allPlayersInLargestRadius = GetPlayersInRadius(m_EventLocation, 150);

        GetTeamsInRadius(allPlayersInLargestRadius);

        foreach (Man player : allPlayersInLargestRadius)
        {
            float distanceSq = vector.DistanceSq(player.GetPosition(), m_EventLocation);

            if (distanceSq <= 2500.0)         // 50^2
            {
                ProcessPlayersInZone(player, 100);
            }
            else if (distanceSq <= 10000.0)   // 100^2
            {
                ProcessPlayersInZone(player, 50);
            }
            else                              // >100m and ≤150m
            {
                ProcessPlayersInZone(player, 20);
            }
        }
    }

    BattlegroundsLocation GetNextLocation()
    {
        if (m_EventLocations.Count() == 0) return null;
        if (m_CurrentLocationIndex == -1)
        {
            m_CurrentLocationIndex = Math.RandomInt(0, m_EventLocations.Count());
        }
        else
        {
            m_CurrentLocationIndex += m_CycleDirection;
        }

        if (m_CurrentLocationIndex >= m_EventLocations.Count())
        {
            m_CurrentLocationIndex = 0;
        }
        else if (m_CurrentLocationIndex < 0)
        {
            m_CurrentLocationIndex = m_EventLocations.Count() - 1;
        }

        BattlegroundsLocation chosenLocation = m_EventLocations[m_CurrentLocationIndex];

        m_CurrentLocationString = chosenLocation.GetName();

        return chosenLocation;
    }

    override void SetCurrentLocationString(string locationName)
    {
        super.SetCurrentLocationString(locationName);
        m_CurrentLocationName = locationName;
    }

    string GetCurrentLocationName()
    {
        return m_CurrentLocationName;
    }

    override float GetCaptureProgress()
    {
        return m_CaptureProgress;
    }

	string GetPlayersListString(array<string> players)
    {
        string playersList = "";
        for (int i = 0; i < players.Count(); i++)
        {
            if (i != 0)
            {
                playersList += ", ";
            }

            playersList += players[i];
        }

        return playersList;
    }

	void InitLocations()
    {
        m_EventLocations.Insert(new BattlegroundsLocation("1270.602661 217.818695 8076.089844", "Topolin Farm"));
        m_EventLocations.Insert(new BattlegroundsLocation("1319.692749 184.686951 7722.111328", "Topolin North"));
        m_EventLocations.Insert(new BattlegroundsLocation("1659.337158 182.001266 7417.352051", "Topolin Fire Station"));
        m_EventLocations.Insert(new BattlegroundsLocation("1804.083374 178.615524 7689.337402", "Topolin Headquarters"));
        m_EventLocations.Insert(new BattlegroundsLocation("2014.200439 208.314285 7394.321289", "Topolin Gas Station"));
        m_EventLocations.Insert(new BattlegroundsLocation("3089.055420 250.098297 7061.735840", "Adamow Farm"));
        m_EventLocations.Insert(new BattlegroundsLocation("3171.017822 233.797623 6790.727539", "Adamow"));
        m_EventLocations.Insert(new BattlegroundsLocation("4411.507813 316.136597 6690.143555", "Muratyn Military"));
        m_EventLocations.Insert(new BattlegroundsLocation("4516.057129 300.848724 6462.737305", "Maratyn"));
        m_EventLocations.Insert(new BattlegroundsLocation("5248.810059 372.467682 5580.659180", "Huta"));
        m_EventLocations.Insert(new BattlegroundsLocation("5886.853516 380.538818 5046.288574", "Drewniki"));
        m_EventLocations.Insert(new BattlegroundsLocation("5944.760742 396.133636 4237.894043", "Nadbor Apartments"));
        m_EventLocations.Insert(new BattlegroundsLocation("5648.504883 431.127472 3795.705811", "Nadbor Military"));
        m_EventLocations.Insert(new BattlegroundsLocation("6191.593262 382.853973 3996.547119", "Nadbor Fire Station"));
        m_EventLocations.Insert(new BattlegroundsLocation("6470.389648 308.560791 4670.072266", "Nadbor Industrial"));
        m_EventLocations.Insert(new BattlegroundsLocation("7021.514648 285.552734 4635.771973", "Nadbor Quarry"));
        m_EventLocations.Insert(new BattlegroundsLocation("7608.786133 249.387817 5198.661133", "Roztoka"));
        m_EventLocations.Insert(new BattlegroundsLocation("7654.977051 242.117203 5951.061523", "Randunin Military"));
        m_EventLocations.Insert(new BattlegroundsLocation("8432.622070 276.644348 6885.439941", "Lembork Ruins"));
        m_EventLocations.Insert(new BattlegroundsLocation("9117.985352 308.905823 6657.993652", "Lembork Military"));
        m_EventLocations.Insert(new BattlegroundsLocation("10138.356445 286.537659 6863.055664", "Karlin"));
        m_EventLocations.Insert(new BattlegroundsLocation("10644.769531 237.372818 6930.666992", "Karlin Farm"));
        m_EventLocations.Insert(new BattlegroundsLocation("11587.012695 264.770935 7013.382813", "Powerplant"));
        m_EventLocations.Insert(new BattlegroundsLocation("11871.341797 340.007233 6559.693848", "Polkrabiec Castle"));
        m_EventLocations.Insert(new BattlegroundsLocation("11870.290039 226.114243 7398.881836", "Ivetow"));
        m_EventLocations.Insert(new BattlegroundsLocation("11278.474609 213.435852 7940.911621", "Ivetow North"));
        m_EventLocations.Insert(new BattlegroundsLocation("10995.662109 194.446899 8992.197266", "Sitnik Industrial"));
        m_EventLocations.Insert(new BattlegroundsLocation("11370.852539 182.252472 9470.278320", "Sitnik"));
        m_EventLocations.Insert(new BattlegroundsLocation("11338.612305 172.194458 10050.394531", "Sitnickie Lake"));
        m_EventLocations.Insert(new BattlegroundsLocation("10757.523438 188.639755 10788.185547", "Grabin South"));
        m_EventLocations.Insert(new BattlegroundsLocation("10923.257813 177.070984 11224.966797", "Grabin Church"));
        m_EventLocations.Insert(new BattlegroundsLocation("10540.193359 179.653366 11124.901367", "Grabin Police Station"));
        m_EventLocations.Insert(new BattlegroundsLocation("9695.612305 228.644669 10560.217773", "Tarnow Farm"));
        m_EventLocations.Insert(new BattlegroundsLocation("9508.803711 242.125641 10345.035156", "Tarnow South"));
        m_EventLocations.Insert(new BattlegroundsLocation("9299.191406 199.562469 10860.996094", "Tarnow Police Station"));
        m_EventLocations.Insert(new BattlegroundsLocation("8293.041992 205.578003 11193.033203", "Tarnow Shooting Range"));
        m_EventLocations.Insert(new BattlegroundsLocation("8130.875000 204.507111 10926.480469", "Tarnow Military"));
        m_EventLocations.Insert(new BattlegroundsLocation("7890.590332 321.299011 9846.443359", "Krsnik Military"));
        m_EventLocations.Insert(new BattlegroundsLocation("7801.644531 339.132446 10080.293945", "Krsnik Radio Tower"));
        m_EventLocations.Insert(new BattlegroundsLocation("6739.469238 177.502441 10853.134766", "Brena Train Station"));
        m_EventLocations.Insert(new BattlegroundsLocation("6482.293457 188.344757 11360.122070", "Brena Police Station"));
        m_EventLocations.Insert(new BattlegroundsLocation("6377.959473 174.648026 10975.930664", "Brena Gas Station"));
        m_EventLocations.Insert(new BattlegroundsLocation("5234.936035 190.698151 10354.456055", "Gliniska Farm"));
        m_EventLocations.Insert(new BattlegroundsLocation("5173.732422 196.462509 9937.312500", "Gliniska Church"));
        m_EventLocations.Insert(new BattlegroundsLocation("5579.032227 310.692932 8817.849609", "Kopa Prison Yard"));
        m_EventLocations.Insert(new BattlegroundsLocation("5502.273438 307.282410 8692.839844", "Kopa Prison Barracks"));
        m_EventLocations.Insert(new BattlegroundsLocation("4939.270996 272.495300 7892.847168", "Olszanka North"));
        m_EventLocations.Insert(new BattlegroundsLocation("4733.685059 291.692444 7594.529297", "Olszanka"));
        m_EventLocations.Insert(new BattlegroundsLocation("3831.061768 202.637741 8479.273438", "Radacz Farm"));
        m_EventLocations.Insert(new BattlegroundsLocation("3590.669434 171.092178 9088.007813", "Jantar Crossing"));
        m_EventLocations.Insert(new BattlegroundsLocation("4085.243408 238.261215 10220.822266", "Lukow Airfield Hangar"));
        m_EventLocations.Insert(new BattlegroundsLocation("4221.795410 247.358032 10404.241211", "Lukow Airfield ATC"));
        m_EventLocations.Insert(new BattlegroundsLocation("3904.209961 249.278427 11597.119141", "Lukow South"));
        m_EventLocations.Insert(new BattlegroundsLocation("3672.529541 252.029419 11786.084961", "Lukow Police Station"));
        m_EventLocations.Insert(new BattlegroundsLocation("3115.671631 262.564453 11985.291016", "Lukow Farm"));
        m_EventLocations.Insert(new BattlegroundsLocation("2087.477783 287.763763 11158.152344", "Bielawa Military North"));
        m_EventLocations.Insert(new BattlegroundsLocation("2132.102295 280.565765 10901.321289", "Bielawa Military South"));
        m_EventLocations.Insert(new BattlegroundsLocation("1683.392212 268.052216 9768.190430", "Bielawa"));
        m_EventLocations.Insert(new BattlegroundsLocation("2017.778198 252.746048 9489.458984", "Bielawa Farm"));
        m_EventLocations.Insert(new BattlegroundsLocation("1242.728271 260.189545 8974.712891", "Bielawa Quarry"));
    }
}