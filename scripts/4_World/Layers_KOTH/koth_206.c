class koth_206 extends KOTH_Base // muratyn to olszanka
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

            Print("[Battlegrounds v206] New Event: " + GetCurrentLocationName());
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
        m_EventLocations.Insert(new BattlegroundsLocation("4686.624512 324.697021 6758.186523", "Checkpoint [Muratyn]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4615.436523 319.521698 6571.416992", "Eastside [Muratyn]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4578.185547 301.122253 6387.083984", "Downtown [Muratyn]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4425.702148 297.723541 6469.337402", "Westside [Muratyn]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4442.941406 316.132477 6680.447754", "Military [Muratyn]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4854.179199 311.614838 7274.731934", "Farm [Olszanka]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4947.176270 303.088684 7488.083008", "Southside [Olszanka]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5006.036133 300.502197 7668.298340", "Outskirts [Olszanka]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4940.889648 272.521393 7894.511230", "Northside [Olszanka]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4837.501465 270.368286 7756.566406", "Lower [Olszanka]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4783.934082 305.336700 7520.812988", "Church [Olszanka]"));
    }
}