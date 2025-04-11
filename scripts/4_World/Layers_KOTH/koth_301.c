class koth_301 extends KOTH_Base // Battle of Sakhal
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

            Print("[Battlegrounds v301] New Event: " + GetCurrentLocationName());
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
        m_EventLocations.Insert(new BattlegroundsLocation("5644.184082 3.114455 4631.619141", "Military Island East"));
        m_EventLocations.Insert(new BattlegroundsLocation("5505.689941 11.789651 3978.152832", "Military Island East"));
        m_EventLocations.Insert(new BattlegroundsLocation("5337.205566 2.979620 4131.645020", "Military Island Storage"));
        m_EventLocations.Insert(new BattlegroundsLocation("5302.530273 2.002819 4671.306152", "Military Island Storage"));
        m_EventLocations.Insert(new BattlegroundsLocation("4961.801758 1.695475 4766.085938", "Military Island Storage"));
        m_EventLocations.Insert(new BattlegroundsLocation("4708.364258 3.572500 5000.699707", "Military Island"));
        m_EventLocations.Insert(new BattlegroundsLocation("3668.725586 5.532546 5488.746094", "Military Island"));
        m_EventLocations.Insert(new BattlegroundsLocation("3016.837402 33.262463 5852.780273", "Military Island"));
        m_EventLocations.Insert(new BattlegroundsLocation("2579.753174 15.225388 5964.234375", "Military Island Bunker"));
        m_EventLocations.Insert(new BattlegroundsLocation("2383.053711 30.939991 6084.524414", "Military Island"));
        m_EventLocations.Insert(new BattlegroundsLocation("2717.529297 23.106548 6078.879883", "Military Island"));
        m_EventLocations.Insert(new BattlegroundsLocation("3404.620850 8.382470 6728.836426", "Military Island Harbour"));
        m_EventLocations.Insert(new BattlegroundsLocation("3297.171631 18.444689 7157.321777", "Military Island Harbour"));
        m_EventLocations.Insert(new BattlegroundsLocation("2690.814941 10.354272 7457.537109", "Military Island Harbour"));
        m_EventLocations.Insert(new BattlegroundsLocation("1974.060425 30.295589 7013.095215", "Military Island"));
        m_EventLocations.Insert(new BattlegroundsLocation("972.659668 1.832500 7714.561523", "Military Island Port"));
        m_EventLocations.Insert(new BattlegroundsLocation("853.117493 1.832500 7738.341797", "Military Island Port"));
        m_EventLocations.Insert(new BattlegroundsLocation("1634.099854 13.990826 8046.564941", "Military Island Ruins"));
        m_EventLocations.Insert(new BattlegroundsLocation("2337.838867 3.704013 8599.904297", "Military Island"));
        m_EventLocations.Insert(new BattlegroundsLocation("2720.206787 8.580346 9220.711914", "Military Island"));
        m_EventLocations.Insert(new BattlegroundsLocation("3655.782715 13.062871 9352.332031", "Military Island"));
        m_EventLocations.Insert(new BattlegroundsLocation("4374.937500 5.642501 8896.416016", "Military Checkpoint"));
        m_EventLocations.Insert(new BattlegroundsLocation("5200.855469 10.932499 9780.861328", "Capital City South"));
        m_EventLocations.Insert(new BattlegroundsLocation("5519.570801 2.209959 10115.678711", "Capital City School"));
        m_EventLocations.Insert(new BattlegroundsLocation("5570.599121 3.185787 10736.297852", "Capital City Harbour"));
        m_EventLocations.Insert(new BattlegroundsLocation("5376.036621 10.106805 11083.161133", "Capital City Harbour"));
        m_EventLocations.Insert(new BattlegroundsLocation("5011.717285 15.137946 11363.877930", "Capital City Industrial"));
        m_EventLocations.Insert(new BattlegroundsLocation("5665.822754 10.952500 11024.636719", "Capital City Apartments"));
        m_EventLocations.Insert(new BattlegroundsLocation("6277.842285 51.683067 10352.149414", "Capital City East"));
        m_EventLocations.Insert(new BattlegroundsLocation("7415.557129 195.532516 9757.210938", "Military Base"));
        m_EventLocations.Insert(new BattlegroundsLocation("7527.744141 205.472519 9654.593750", "Military Base"));
        m_EventLocations.Insert(new BattlegroundsLocation("8305.107422 30.987940 10337.801758", "Powerplant"));
        m_EventLocations.Insert(new BattlegroundsLocation("8429.151367 42.919220 10299.130859", "Powerplant"));
        m_EventLocations.Insert(new BattlegroundsLocation("8136.992676 98.479027 11000.380859", "Logging Camp"));
        m_EventLocations.Insert(new BattlegroundsLocation("8167.725586 216.892502 11980.206055", "Military Base"));
        m_EventLocations.Insert(new BattlegroundsLocation("8786.283203 298.140442 12759.124023", "Radio Abpopa"));
        m_EventLocations.Insert(new BattlegroundsLocation("9310.333984 14.434160 13710.315430", "Northern City Industrial"));
        m_EventLocations.Insert(new BattlegroundsLocation("9475.184570 24.498295 13429.104492", "Northern City Apartments"));
        m_EventLocations.Insert(new BattlegroundsLocation("9697.583008 3.862468 13540.172852", "Northern City"));
        m_EventLocations.Insert(new BattlegroundsLocation("9884.524414 2.322500 13535.660156", "Northern City Storage"));
        m_EventLocations.Insert(new BattlegroundsLocation("10348.462891 11.881780 13345.510742", "Northern City East"));
        m_EventLocations.Insert(new BattlegroundsLocation("10935.518555 20.152428 13240.393555", "Northern Village"));
        m_EventLocations.Insert(new BattlegroundsLocation("11394.391602 111.121574 12337.000977", "North Valley Farm"));
        m_EventLocations.Insert(new BattlegroundsLocation("11030.516602 350.392487 11498.022461", "Research Facility"));
        m_EventLocations.Insert(new BattlegroundsLocation("12067.498047 216.185501 11689.251953", "Northern Dam"));
        m_EventLocations.Insert(new BattlegroundsLocation("13283.736328 9.967641 12002.446289", "Ore Harbour"));
        m_EventLocations.Insert(new BattlegroundsLocation("13595.405273 2.562497 12279.130859", "Ore Harbour"));
        m_EventLocations.Insert(new BattlegroundsLocation("13578.469727 1.694192 12092.296875", "Ore Harbour Boat"));
        m_EventLocations.Insert(new BattlegroundsLocation("14061.388672 2.577340 12268.636719", "Ore Harbour"));
        m_EventLocations.Insert(new BattlegroundsLocation("13703.416016 2.562500 11783.459961", "Ore Harbour Checkpoint"));
        m_EventLocations.Insert(new BattlegroundsLocation("13421.829102 11.791530 11647.443359", "Ore Harbour"));
        m_EventLocations.Insert(new BattlegroundsLocation("12321.549805 332.469391 10788.912109", "Quarry"));
        m_EventLocations.Insert(new BattlegroundsLocation("11690.337891 123.792305 9870.165039", "Quarry"));
        m_EventLocations.Insert(new BattlegroundsLocation("11051.423828 372.892487 8682.813477", "Mountain Radio Towers"));
        m_EventLocations.Insert(new BattlegroundsLocation("10407.337891 258.266876 8568.286133", "Military Base"));
        m_EventLocations.Insert(new BattlegroundsLocation("10357.899414 245.322479 8548.637695", "Military Base"));
        m_EventLocations.Insert(new BattlegroundsLocation("9778.933594 235.307800 8491.483398", "Avalanche Clearing"));
        m_EventLocations.Insert(new BattlegroundsLocation("8825.880859 69.436005 7931.629883", "South Valley Farm"));
        m_EventLocations.Insert(new BattlegroundsLocation("7848.377930 6.192500 7725.713379", "Airfield City PD"));
        m_EventLocations.Insert(new BattlegroundsLocation("7699.338867 2.599963 7936.422852", "Airfield City Apartments"));
        m_EventLocations.Insert(new BattlegroundsLocation("7394.766113 2.758584 7799.770508", "Airfield City Industrial"));
        m_EventLocations.Insert(new BattlegroundsLocation("7358.024902 11.676888 7295.518555", "Airfield HQ"));
        m_EventLocations.Insert(new BattlegroundsLocation("7043.340332 2.727745 7469.690430", "Airfield ATC"));
        m_EventLocations.Insert(new BattlegroundsLocation("6913.062012 2.472500 7147.650879", "Airfield Tents"));
        m_EventLocations.Insert(new BattlegroundsLocation("6332.964844 1.628651 6143.853516", "Shipwreck"));
    }
}