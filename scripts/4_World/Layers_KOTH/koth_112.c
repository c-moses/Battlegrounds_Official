class koth_112 extends KOTH_Base // Battle of Pavlovo
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

            Print("[Battlegrounds v112] New Event: " + GetCurrentLocationName());
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
        int numPlayersInZone = GetCEApi().CountPlayersWithinRange(m_EventLocation, 100);

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
        array<Man> allPlayersInLargestRadius = GetPlayersInRadius(m_EventLocation, 100);

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
            else                             // >50m and ≤100m
            {
                ProcessPlayersInZone(player, 20);
            }
        }
    }

	BattlegroundsLocation GetNextLocation()
	{
		if (m_EventLocations.Count() == 0) return null;

		m_CurrentLocationIndex++;

		if (m_CurrentLocationIndex >= m_EventLocations.Count())
		{
			m_CurrentLocationIndex = 0;
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
        m_EventLocations.Insert(new BattlegroundsLocation("1847.064575 129.580750 3454.619141", "Military Entrance [Pavlovo]"));
		m_EventLocations.Insert(new BattlegroundsLocation("2085.132324 92.107277 3284.468750", "Military Checkpoint [Pavlovo]"));
		m_EventLocations.Insert(new BattlegroundsLocation("2161.900391 95.388847 3358.117432", "Military Hospital [Pavlovo]"));
		m_EventLocations.Insert(new BattlegroundsLocation("2078.180420 109.614838 3519.259277", "Military North [Pavlovo]"));
		m_EventLocations.Insert(new BattlegroundsLocation("1764.830688 141.805389 3811.804199", "Town Church [Pavlovo]"));
		m_EventLocations.Insert(new BattlegroundsLocation("1711.322510 148.065323 3999.218262", "Town North [Pavlovo]"));
		m_EventLocations.Insert(new BattlegroundsLocation("1660.384521 144.047897 3865.971436", "Town Backyards [Pavlovo]"));
		m_EventLocations.Insert(new BattlegroundsLocation("1732.933228 137.945053 3614.063721", "Town South [Pavlovo]"));
    }
}