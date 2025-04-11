class koth_203 extends KOTH_Base // Tarnow
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

            Print("[Battlegrounds v203] New Event: " + GetCurrentLocationName());
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

        // No previous event, start at Headquarters
        if (!m_PreviousEventPositionSet)
        {
            BattlegroundsLocation bridgeLocation = GetLocationByName("Apartments [Tarnow]");
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

        int nextIndex = currentIndex + m_Direction;

        // Wrap around when reaching the end of the array
        if (nextIndex >= m_EventLocations.Count())
        {
            nextIndex = 0; // Go back to the first element
        }

        // Wrap around when going below the start of the array
        if (nextIndex < 0)
        {
            nextIndex = m_EventLocations.Count() - 1; // Go to the last element
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
        m_EventLocations.Insert(new BattlegroundsLocation("9358.428711 212.442459 10739.147461", "Apartments [Tarnow]"));
        m_EventLocations.Insert(new BattlegroundsLocation("9301.420898 199.547729 10864.031250", "Police Dept [Tarnow]"));
        m_EventLocations.Insert(new BattlegroundsLocation("9285.480469 197.362122 10976.264648", "Northside [Tarnow]"));
        m_EventLocations.Insert(new BattlegroundsLocation("9306.817383 186.748047 11125.312500", "Industrial [Tarnow]"));
        m_EventLocations.Insert(new BattlegroundsLocation("9364.317383 196.335114 11012.584961", "Fire Dept [Tarnow]"));
        m_EventLocations.Insert(new BattlegroundsLocation("9371.791992 196.299927 10871.723633", "Orchard [Tarnow]"));
        m_EventLocations.Insert(new BattlegroundsLocation("9614.900391 209.599091 10806.325195", "Sawmill [Tarnow]"));
        m_EventLocations.Insert(new BattlegroundsLocation("9654.327148 220.193130 10590.054688", "Farm [Tarnow]"));
        m_EventLocations.Insert(new BattlegroundsLocation("9579.471680 239.982346 10415.360352", "Southside [Tarnow]"));
        m_EventLocations.Insert(new BattlegroundsLocation("9526.639648 243.850281 10311.451172", "Cemetery [Tarnow]"));
        m_EventLocations.Insert(new BattlegroundsLocation("9410.196289 242.036774 10379.227539", "Garages [Tarnow]"));
        m_EventLocations.Insert(new BattlegroundsLocation("9395.053711 231.517273 10565.806641", "Main Street [Tarnow]"));
    }
}