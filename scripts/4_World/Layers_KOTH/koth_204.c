class koth_204 extends KOTH_Base // nadbor
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

            Print("[Battlegrounds v204] New Event: " + GetCurrentLocationName());
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
        if (m_EventLocations.Count() == 0)
        {
            return null;
        }

        // No previous event, start at Headquarters
        if (!m_PreviousEventPositionSet)
        {
            BattlegroundsLocation bridgeLocation = GetLocationByName("Police Dept [Nadbor]");
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
        m_EventLocations.Insert(new BattlegroundsLocation("5941.367188 397.116302 4139.842285", "Police Dept [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5817.253418 406.050598 4207.664063", "Construction [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5730.510742 416.579987 4087.179199", "Amusement Park [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5644.830078 422.142578 3858.698486", "Military Barracks [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5552.428223 420.522430 3767.864990", "Military Storage [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5687.974121 419.572479 3737.572021", "Military Cargo [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5851.938965 398.320618 3866.128418", "Southside [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5992.030273 392.562347 3966.112061", "Lumberyard [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("6085.346680 388.907593 4056.714111", "Apartments [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("6191.374023 387.778900 3993.581543", "Fire Dept [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("6310.041504 382.410278 4026.764160", "Workshop [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("6265.187012 372.869263 4204.475586", "Northside [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("6320.906250 350.907135 4375.240723", "Ruins [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("6423.870605 319.196381 4586.266113", "Quarry Gate [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("6488.819336 308.430237 4731.318359", "Quarry Factory [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("6355.043457 298.782349 4859.373535", "Quarry [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("6382.943359 311.372375 4691.274414", "Quarry Storage [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("6269.871094 364.262115 4290.319336", "Outskirts [Nadbor]"));
        m_EventLocations.Insert(new BattlegroundsLocation("6118.562012 386.282471 4169.070801", "Downtown [Nadbor]"));
    }
}