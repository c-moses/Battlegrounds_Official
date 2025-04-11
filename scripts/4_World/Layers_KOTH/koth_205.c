class koth_205 extends KOTH_Base // Tarnow
{
    vector m_EventPosition;
    vector m_PreviousEventPosition;
    bool m_PreviousEventPositionSet;
    string m_CurrentLocationName, m_LastLocationName, m_SecondLastLocationName;
    protected ref array<Object> m_CurrentObjects = new array<Object> ();
	private bool m_LastCheckedPlayerInZone = false;
    const float CAPTURE_PROGRESS_CHECK_INTERVAL = 1.0;
    const float PLAYERS_IN_RADIUS_CHECK_INTERVAL = 10.0;
    private int m_Direction = 1;

    override void InitLayer(string currentLayer, float captureTime) 
    {
        super.InitLayer(currentLayer, captureTime);

        InitLocations();
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

            Print("[Battlegrounds v205] New Event: " + GetCurrentLocationName());
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
        if (m_EventLocations.Count() == 0)
        {
            return null;
        }

        // No previous event, start at Headquarters
        if (!m_PreviousEventPositionSet)
        {
            BattlegroundsLocation bridgeLocation = GetLocationByName("Bridge [Prison]");
            m_CurrentLocationString = bridgeLocation.GetName();  // Set the current location string
            if (Math.RandomInt(0, 2) == 0)
            {
                m_Direction = 1;
            }
            else
            {
                m_Direction = -1;
            }
            return bridgeLocation;
        }
        
        int currentIndex = -1;

        // Index the current location
        for (int i = 0; i < m_EventLocations.Count(); i++)
        {
            if (m_EventLocations[i].GetName() == m_CurrentLocationName)
            {
                currentIndex = i;
                break;
            }
        }

        if (currentIndex == -1)
        {
            return null;
        }

        // If at the start or end of the array, return to Headquarters
        if (currentIndex == 0 || currentIndex == m_EventLocations.Count() - 1)
        {
            BattlegroundsLocation supermarketLocation = GetLocationByName("Bridge [Prison]");
            m_CurrentLocationString = supermarketLocation.GetName();
            m_Direction = -m_Direction; // Reverse direction after visiting the Headquarters
            return supermarketLocation;
        }

        int nextIndex = currentIndex + m_Direction;

        BattlegroundsLocation nextLocation = m_EventLocations[nextIndex];
        m_CurrentLocationString = nextLocation.GetName();  // Set the current location string

        return nextLocation;
    }

    BattlegroundsLocation GetLocationByName(string name)
    {
        for (int i = 0; i < m_EventLocations.Count(); i++)
        {
            if (m_EventLocations[i].GetName() == name)
                return m_EventLocations[i];
        }
        return null;
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
        m_EventLocations.Insert(new BattlegroundsLocation("5577.752441 310.655548 8817.831055", "Yard [Prison]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5548.379395 315.281311 8800.771484", "Offices [Prison]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5513.803223 315.367981 8779.733398", "Bridge [Prison]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5488.177734 318.838226 8748.188477", "Third Floor [Prison]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5491.373535 307.282440 8707.746094", "Parking Lot [Prison]"));
    }
}