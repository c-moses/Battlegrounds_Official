class koth_108 extends KOTH_Base // Battle of Zelenogorsk
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

            Print("[Battlegrounds v108] New Event: " + GetCurrentLocationName());
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
        int numPlayersInZone = GetCEApi().CountPlayersWithinRange(m_EventLocation, 75);

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
        array<Man> allPlayersInLargestRadius = GetPlayersInRadius(m_EventLocation, 75);

        GetTeamsInRadius(allPlayersInLargestRadius);

        foreach (Man player : allPlayersInLargestRadius)
        {
            float distanceSq = vector.DistanceSq(player.GetPosition(), m_EventLocation);

            if (distanceSq <= 625.0)         // 25^2
            {
                ProcessPlayersInZone(player, 100);
            }
            else if (distanceSq <= 2500.0)   // 50^2
            {
                ProcessPlayersInZone(player, 50);
            }
            else                             // >50m and ≤75m
            {
                ProcessPlayersInZone(player, 20);
            }
        }
    }

    BattlegroundsLocation GetNextLocation()
    {
        if (m_EventLocations.Count() == 0) return null;

        m_CurrentLocationIndex += m_CycleDirection;

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
        m_EventLocations.Insert(new BattlegroundsLocation("2722.938965 206.579178 5308.805664", "Town Center [Zelenogorsk]"));
		m_EventLocations.Insert(new BattlegroundsLocation("2726.321289 200.169128 5183.473145", "Clinic [Zelenogorsk]"));
		m_EventLocations.Insert(new BattlegroundsLocation("2590.975342 195.095810 5090.545410", "Fire Station [Zelenogorsk]"));
		m_EventLocations.Insert(new BattlegroundsLocation("2461.335938 193.195313 5116.033203", "Military [Zelenogorsk]"));
		m_EventLocations.Insert(new BattlegroundsLocation("2479.936523 190.301254 5248.568359", "Train Station [Zelenogorsk]"));
        m_EventLocations.Insert(new BattlegroundsLocation("2548.981689 200.933167 5401.745605", "Orchard [Zelenogorsk]"));
		m_EventLocations.Insert(new BattlegroundsLocation("2610.310547 211.180222 5548.140625", "Water Tower [Zelenogorsk]"));
		m_EventLocations.Insert(new BattlegroundsLocation("2733.647949 209.906006 5462.957031", "Church [Zelenogorsk]"));
    }
}