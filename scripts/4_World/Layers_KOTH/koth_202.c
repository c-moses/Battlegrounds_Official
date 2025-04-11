class koth_202 extends KOTH_Base // Topolin
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

            Print("[Battlegrounds v202] New Event: " + GetCurrentLocationName());
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
            float distanceToEvent = vector.Distance(player.GetPosition(), m_EventLocation);
            
            if (distanceToEvent <= 50.0)
            {
                ProcessPlayersInZone(player, 100);
            }
            else if (distanceToEvent <= 100.0)
            {
                ProcessPlayersInZone(player, 50);
            }
            else 
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

        // no previous event, start at Fire Station
        if (!m_PreviousEventPositionSet)
        {
            BattlegroundsLocation bridgeLocation = GetLocationByName("Fire Station [Topolin]");
            m_CurrentLocationString = bridgeLocation.GetName();  // Set the current location string
            return bridgeLocation;
        }
        
        int currentIndex = -1;

        // index the current location
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

        // reverse direction
        if (currentIndex == 0 || currentIndex == m_EventLocations.Count() - 1)
        {
            m_Direction = -m_Direction;
        }

        int nextIndex = currentIndex + m_Direction;

        if (m_CurrentLocationName == "Fire Station [Topolin]")
        {
            int randomChoice = Math.RandomInt(0, 2);
            if (randomChoice == 0)
            {
                nextIndex = currentIndex + 1;
                m_Direction = 1;
            }
            else
            {
                nextIndex = currentIndex - 1;
                m_Direction = -1;
            }
        }

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
        m_EventLocations.Insert(new BattlegroundsLocation("1296.610107 186.502563 7400.705078", "Restaurant [Topolin]"));
        m_EventLocations.Insert(new BattlegroundsLocation("1486.122559 183.630005 7397.256836", "Police Station [Topolin]"));
        m_EventLocations.Insert(new BattlegroundsLocation("1565.625366 175.884369 7541.600586", "Industrial [Topolin]"));
        m_EventLocations.Insert(new BattlegroundsLocation("1656.717773 181.892365 7423.999512", "Fire Station [Topolin]"));
        m_EventLocations.Insert(new BattlegroundsLocation("1805.586304 189.081787 7435.562012", "Construction [Topolin]"));
        m_EventLocations.Insert(new BattlegroundsLocation("1829.730103 209.173126 7318.917480", "Headquarters [Topolin]"));
        m_EventLocations.Insert(new BattlegroundsLocation("1747.336792 219.512360 7199.587402", "Church [Topolin]"));
    }
}