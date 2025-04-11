class koth_101 extends KOTH_Base // Chernarus Full
{
    vector m_EventPosition;
    vector m_PreviousEventPosition;
    bool m_PreviousEventPositionSet;
    string m_CurrentLocationName, m_LastLocationName, m_SecondLastLocationName;
    protected ref array<Object> m_CurrentObjects = new array<Object> ();
    private bool m_LastCheckedPlayerInZone = false;
    const float CAPTURE_PROGRESS_CHECK_INTERVAL = 1.0;
    const float PLAYERS_IN_RADIUS_CHECK_INTERVAL = 10.0;

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
        
        BattlegroundsLocation location = GetRandomLocation();
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

            Print("[Battlegrounds v101] New Event: " + GetCurrentLocationName());
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
        int numPlayersInZone = GetCEApi().CountPlayersWithinRange(m_EventLocation, 300);

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
        array<Man> allPlayersInLargestRadius = GetPlayersInRadius(m_EventLocation, 300);

        GetTeamsInRadius(allPlayersInLargestRadius);

        foreach (Man player : allPlayersInLargestRadius)
        {
            float distanceToEvent = vector.Distance(player.GetPosition(), m_EventLocation);
            
            if (distanceToEvent <= 100.0)
            {
                ProcessPlayersInZone(player, 100);
            }
            else if (distanceToEvent <= 200.0)
            {
                ProcessPlayersInZone(player, 50);
            }
            else 
            {
                ProcessPlayersInZone(player, 20);
            }
        }
    }

    void InitLocations()
    {
        m_EventLocations.Insert(new BattlegroundsLocation("1730.306885 451.742432 14283.429688", "Tisy East Gate"));
        m_EventLocations.Insert(new BattlegroundsLocation("1514.261963 439.021484 13590.531250", "Tisy Tents"));
        m_EventLocations.Insert(new BattlegroundsLocation("1414.361450 435.816864 12922.901367", "Hilltop Rally"));
        m_EventLocations.Insert(new BattlegroundsLocation("1466.138550 283.085327 11953.317383", "Sinystok"));
        m_EventLocations.Insert(new BattlegroundsLocation("1270.706543 294.042450 11506.480469", "Ski Resort"));
        m_EventLocations.Insert(new BattlegroundsLocation("500.294495 421.738312 11059.052734", "Biathlon Arena"));
        m_EventLocations.Insert(new BattlegroundsLocation("421.138580 410.703491 10391.458008", "Zabolotye Backroads"));
        m_EventLocations.Insert(new BattlegroundsLocation("359.656769 294.825165 9405.576172", "Tri Kresta Military"));
        m_EventLocations.Insert(new BattlegroundsLocation("693.351501 194.167694 8690.119141", "Rostoki Train Bridge"));
        m_EventLocations.Insert(new BattlegroundsLocation("920.123718 180.494019 7778.084961", "Myshkino Sawmill"));
        m_EventLocations.Insert(new BattlegroundsLocation("1165.598877 185.339996 7225.602051", "Myshkino Military"));
        m_EventLocations.Insert(new BattlegroundsLocation("1052.969604 157.468552 6672.069336", "Metalurg Hunting Camp"));
        m_EventLocations.Insert(new BattlegroundsLocation("643.329163 135.148117 5924.300293", "Eagle Lake"));
        m_EventLocations.Insert(new BattlegroundsLocation("1131.165161 187.257507 5002.802246", "Zvir Radio Tower"));
        m_EventLocations.Insert(new BattlegroundsLocation("1373.808105 138.082092 4027.436768", "Pavlovo Farm"));
        m_EventLocations.Insert(new BattlegroundsLocation("2079.662354 92.206070 3291.888916", "Pavlovo Military"));
        m_EventLocations.Insert(new BattlegroundsLocation("1886.126099 7.589051 2274.134766", "Kamenka"));
        m_EventLocations.Insert(new BattlegroundsLocation("2479.870605 3.832595 1424.279785", "Prison Island West"));
        m_EventLocations.Insert(new BattlegroundsLocation("3081.780273 5.437077 1325.286987", "Prison Island East"));
        m_EventLocations.Insert(new BattlegroundsLocation("3601.586670 6.012489 2160.598877", "Komarovo Docks"));
        m_EventLocations.Insert(new BattlegroundsLocation("4447.998047 6.374921 2480.875732", "Balota"));
        m_EventLocations.Insert(new BattlegroundsLocation("5030.681152 9.542486 2355.875488", "Balota Airfield"));
        m_EventLocations.Insert(new BattlegroundsLocation("5695.031250 70.552711 2567.843506", "Vysotovo School"));
        m_EventLocations.Insert(new BattlegroundsLocation("6598.771973 8.389774 2427.128662", "Cherno City Hall"));
        m_EventLocations.Insert(new BattlegroundsLocation("7236.364746 30.415434 3063.441162", "Cherno Military"));
        m_EventLocations.Insert(new BattlegroundsLocation("7765.497070 3.089444 2687.802490", "Cherno Shipwreck"));
        m_EventLocations.Insert(new BattlegroundsLocation("8355.030273 10.835742 2473.961670", "Golova Lighthouse"));
        m_EventLocations.Insert(new BattlegroundsLocation("8821.683594 216.815628 2856.566162", "Kozlovs' Peak"));
        m_EventLocations.Insert(new BattlegroundsLocation("9342.384766 6.012474 2015.677490", "Electro West"));
        m_EventLocations.Insert(new BattlegroundsLocation("10296.793945 12.743051 2282.461670", "Electro Hospital"));
        m_EventLocations.Insert(new BattlegroundsLocation("10594.654297 34.160213 2526.907471", "Electro Church"));
        m_EventLocations.Insert(new BattlegroundsLocation("10349.285156 39.858109 3465.731201", "Topika Dam"));
        m_EventLocations.Insert(new BattlegroundsLocation("11116.470703 304.601563 4092.824463", "Rog Radio Tower"));
        m_EventLocations.Insert(new BattlegroundsLocation("11966.561523 6.406665 3556.591309", "Kamyshovo"));
        m_EventLocations.Insert(new BattlegroundsLocation("13012.714844 3.528998 3435.042969", "Skalisty Strait"));
        m_EventLocations.Insert(new BattlegroundsLocation("13474.765625 80.332909 3324.030029", "Skalisty Island"));
        m_EventLocations.Insert(new BattlegroundsLocation("13747.027344 2.420194 4219.011230", "Cape Krutoy"));
        m_EventLocations.Insert(new BattlegroundsLocation("12885.135742 173.885025 4490.623535", "Tulga"));
        m_EventLocations.Insert(new BattlegroundsLocation("12987.754883 17.255522 5499.489746", "Three Valleys"));
        m_EventLocations.Insert(new BattlegroundsLocation("13361.519531 7.670527 6124.285645", "Solnechny"));
        m_EventLocations.Insert(new BattlegroundsLocation("13165.161133 6.012499 7087.604492", "Solnechny North"));
        m_EventLocations.Insert(new BattlegroundsLocation("12977.101563 21.791351 7958.853516", "Nizhnee"));
        m_EventLocations.Insert(new BattlegroundsLocation("12626.615234 9.773517 8734.875000", "Berezino Scrapyard"));
        m_EventLocations.Insert(new BattlegroundsLocation("11920.546875 54.141514 9046.899414", "Berezino Hospital"));
        m_EventLocations.Insert(new BattlegroundsLocation("12181.332031 11.312416 9754.057617", "Berezino School"));
        m_EventLocations.Insert(new BattlegroundsLocation("12935.304688 5.992491 10176.964844", "Berezino North PD"));
        m_EventLocations.Insert(new BattlegroundsLocation("13047.326172 6.416161 10571.098633", "Berezino North Church"));
        m_EventLocations.Insert(new BattlegroundsLocation("13797.389648 26.589447 11138.464844", "Rify Shipwreck"));
        m_EventLocations.Insert(new BattlegroundsLocation("13908.084961 52.174038 11324.313477", "Rify Hilltop"));
        m_EventLocations.Insert(new BattlegroundsLocation("13406.806641 82.466881 12154.074219", "Black Lake"));
        m_EventLocations.Insert(new BattlegroundsLocation("13345.333008 95.342064 12952.981445", "Olsha"));
        m_EventLocations.Insert(new BattlegroundsLocation("13965.329102 19.567099 13207.936523", "Svetloyarsk"));
        m_EventLocations.Insert(new BattlegroundsLocation("13602.665039 13.117899 14023.579102", "Turovo"));
        m_EventLocations.Insert(new BattlegroundsLocation("12861.069336 19.204769 14172.349609", "Northeast Highway"));
        m_EventLocations.Insert(new BattlegroundsLocation("12160.919922 27.402435 14331.293945", "Novodmitrovsk East"));
        m_EventLocations.Insert(new BattlegroundsLocation("11413.127930 78.545227 14590.172852", "Novodmitrovsk Hospital"));
        m_EventLocations.Insert(new BattlegroundsLocation("10720.474609 53.749374 14348.192383", "Novodmitrovsk West"));
        m_EventLocations.Insert(new BattlegroundsLocation("10083.195313 62.950165 13949.175781", "Pobeda Dam"));
        m_EventLocations.Insert(new BattlegroundsLocation("9283.264648 107.195251 13492.719727", "Klyuch Castle"));
        m_EventLocations.Insert(new BattlegroundsLocation("8456.462891 205.138306 13981.750000", "Arsenovo"));
        m_EventLocations.Insert(new BattlegroundsLocation("7864.786621 343.103027 14694.590820", "Troitskoye"));
        m_EventLocations.Insert(new BattlegroundsLocation("7086.377441 355.552399 14582.300781", "Kamensk Mine"));
        m_EventLocations.Insert(new BattlegroundsLocation("6485.212891 463.612457 14855.518555", "Krasnoye Radio Tower"));
        m_EventLocations.Insert(new BattlegroundsLocation("5894.878418 478.341888 14761.952148", "Skalka Cliffside"));
        m_EventLocations.Insert(new BattlegroundsLocation("4918.613770 450.177795 14855.055664", "Stary Yar Lodge"));
        m_EventLocations.Insert(new BattlegroundsLocation("4316.005371 341.312714 14463.483398", "Cliffside Rally"));
        m_EventLocations.Insert(new BattlegroundsLocation("3677.458740 363.582947 14818.383789", "Pobeda"));
        m_EventLocations.Insert(new BattlegroundsLocation("3334.371338 324.772430 15176.308594", "Tisy Checkpoint"));
        m_EventLocations.Insert(new BattlegroundsLocation("2588.934570 448.003601 14840.119141", "Tisy Lodge"));
    }

    BattlegroundsLocation GetRandomLocation()
    {
        if (m_EventLocations.Count() == 0) return null;

        // Create a list of valid locations
        ref array<ref BattlegroundsLocation> validLocations = new array<ref BattlegroundsLocation>;

        if (m_PreviousEventPositionSet)  // Ensure this variable is set to true after the first location has been picked
        {
            for (int i = 0; i < m_EventLocations.Count(); i++)
            {
                vector potentialLocation = m_EventLocations[i].GetPosition();
                if (vector.Distance(m_PreviousEventPosition, potentialLocation) <= 1060)
                {
                    validLocations.Insert(m_EventLocations[i]);
                }
            }

            if (validLocations.Count() == 0)
            {
                // No valid locations within 1060m, so all locations become valid
                validLocations = m_EventLocations;
            }
        }
        else
        {
            // If no previous event position, all locations are valid
            validLocations = m_EventLocations;
        }

        // Filter out previously used locations
        ref array<ref BattlegroundsLocation> filteredLocations = new array<ref BattlegroundsLocation>;
        for (int j = 0; j < validLocations.Count(); j++)
        {
            if (validLocations[j].GetName() != m_CurrentLocationName && validLocations[j].GetName() != m_LastLocationName && validLocations[j].GetName() != m_SecondLastLocationName)
            {
                filteredLocations.Insert(validLocations[j]);
            }
        }

        // If filtering removed all options, reset to validLocations
        if (filteredLocations.Count() == 0)
        {
            filteredLocations = validLocations;
        }

        // Randomly select a location
        int randomIndex = Math.RandomInt(0, filteredLocations.Count());
        BattlegroundsLocation chosenLocation = filteredLocations[randomIndex];

        // Update the base layer's current location string
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
}

        /*m_EventLocations.Insert(new BattlegroundsLocation("1730.306885 451.742432 14283.429688", "Tisy East Gate [B2-9]"));
        m_EventLocations.Insert(new BattlegroundsLocation("1514.261963 439.021484 13590.531250", "Tisy Tents [B2-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("1414.361450 435.816864 12922.901367", "Hilltop Rally [B3-5]"));
        m_EventLocations.Insert(new BattlegroundsLocation("1466.138550 283.085327 11953.317383", "Sinystok [B4-5]"));
        m_EventLocations.Insert(new BattlegroundsLocation("1270.706543 294.042450 11506.480469", "Ski Resort [A5-8]"));
        m_EventLocations.Insert(new BattlegroundsLocation("500.294495 421.738312 11059.052734", "Biathlon Arena [A5-8]"));
        m_EventLocations.Insert(new BattlegroundsLocation("421.138580 410.703491 10391.458008", "Zabolotye Backroads [A5-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("359.656769 294.825165 9405.576172", "Tri Kresta Military [A6-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("693.351501 194.167694 8690.119141", "Rostoki Train Bridge [A7-6]"));
        m_EventLocations.Insert(new BattlegroundsLocation("920.123718 180.494019 7778.084961", "Myshkino Sawmill [A8-6]"));
        m_EventLocations.Insert(new BattlegroundsLocation("1165.598877 185.339996 7225.602051", "Myshkino Military [B9-7]"));
        m_EventLocations.Insert(new BattlegroundsLocation("1052.969604 157.468552 6672.069336", "Metalurg Hunting Camp [B9-1]"));
        m_EventLocations.Insert(new BattlegroundsLocation("643.329163 135.148117 5924.300293", "Eagle Lake [A10-5]"));
        m_EventLocations.Insert(new BattlegroundsLocation("1131.165161 187.257507 5002.802246", "Zvir Radio Tower [B11-4]"));
        m_EventLocations.Insert(new BattlegroundsLocation("1373.808105 138.082092 4027.436768", "Pavlovo Farm [B12-8]"));
        m_EventLocations.Insert(new BattlegroundsLocation("2079.662354 92.206070 3291.888916", "Pavlovo Military [C13-7]"));
        m_EventLocations.Insert(new BattlegroundsLocation("1886.126099 7.589051 2274.134766", "Kamenka [B14-9]"));
        m_EventLocations.Insert(new BattlegroundsLocation("2479.870605 3.832595 1424.279785", "Prison Island West [C14-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("3081.780273 5.437077 1325.286987", "Prison Island East [D15-7]"));
        m_EventLocations.Insert(new BattlegroundsLocation("3601.586670 6.012489 2160.598877", "Komarovo Docks [D14-8]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4447.998047 6.374921 2480.875732", "Balota [E13-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5030.681152 9.542486 2355.875488", "Balota Airfield [F13-1]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5695.031250 70.552711 2567.843506", "Vysotovo School [F13-3]"));
        m_EventLocations.Insert(new BattlegroundsLocation("6598.771973 8.389774 2427.128662", "Cherno City Hall [G13-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("7236.364746 30.415434 3063.441162", "Cherno Military [H13-7]"));
        m_EventLocations.Insert(new BattlegroundsLocation("7765.497070 3.089444 2687.802490", "Cherno Shipwreck [H13-6]"));
        m_EventLocations.Insert(new BattlegroundsLocation("8355.030273 10.835742 2473.961670", "Golova Lighthouse [I13-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("8821.683594 216.815628 2856.566162", "Kozlovs' Peak [I13-6]"));
        m_EventLocations.Insert(new BattlegroundsLocation("9342.384766 6.012474 2015.677490", "Electro West [J14-5]"));
        m_EventLocations.Insert(new BattlegroundsLocation("10296.793945 12.743051 2282.461670", "Electro Hospital [K14-7]"));
        m_EventLocations.Insert(new BattlegroundsLocation("10594.654297 34.160213 2526.907471", "Electro Church [K13-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("10349.285156 39.858109 3465.731201", "Topika Dam [K12-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("11116.470703 304.601563 4092.824463", "Rog Radio Tower [L12-7]"));
        m_EventLocations.Insert(new BattlegroundsLocation("11966.561523 6.406665 3556.591309", "Kamyshovo [L12-3]"));
        m_EventLocations.Insert(new BattlegroundsLocation("13012.714844 3.528998 3435.042969", "Skalisty Strait [N12-1]"));
        m_EventLocations.Insert(new BattlegroundsLocation("13474.765625 80.332909 3324.030029", "Skalisty Island [N13-8]"));
        m_EventLocations.Insert(new BattlegroundsLocation("13747.027344 2.420194 4219.011230", "Cape Krutoy [N12-9]"));
        m_EventLocations.Insert(new BattlegroundsLocation("12885.135742 173.885025 4490.623535", "Tulga [M11-3]"));
        m_EventLocations.Insert(new BattlegroundsLocation("12987.754883 17.255522 5499.489746", "Three Valleys [M10-3]"));
        m_EventLocations.Insert(new BattlegroundsLocation("13361.519531 7.670527 6124.285645", "Solnechny [N10-8]"));
        m_EventLocations.Insert(new BattlegroundsLocation("13165.161133 6.012499 7087.604492", "Solnechny North [N9-7]"));
        m_EventLocations.Insert(new BattlegroundsLocation("12977.101563 21.791351 7958.853516", "Nizhnee [M8-6]"));
        m_EventLocations.Insert(new BattlegroundsLocation("12626.615234 9.773517 8734.875000", "Berezino Scrapyard [M7-5]"));
        m_EventLocations.Insert(new BattlegroundsLocation("11920.546875 54.141514 9046.899414", "Berezino Hospital [L7-9]"));
        m_EventLocations.Insert(new BattlegroundsLocation("12181.332031 11.312416 9754.057617", "Berezino School [M6-4]"));
        m_EventLocations.Insert(new BattlegroundsLocation("12935.304688 5.992491 10176.964844", "Berezino North PD [M6-9]"));
        m_EventLocations.Insert(new BattlegroundsLocation("13047.326172 6.416161 10571.098633", "Berezino North Church [N5-1]"));
        m_EventLocations.Insert(new BattlegroundsLocation("13797.389648 26.589447 11138.464844", "Rify Shipwreck [N5-9]"));
        m_EventLocations.Insert(new BattlegroundsLocation("13908.084961 52.174038 11324.313477", "Rify Hilltop [N5-9]"));
        m_EventLocations.Insert(new BattlegroundsLocation("13406.806641 82.466881 12154.074219", "Black Lake [N4-8]"));
        m_EventLocations.Insert(new BattlegroundsLocation("13345.333008 95.342064 12952.981445", "Olsha [N3-5]"));
        m_EventLocations.Insert(new BattlegroundsLocation("13965.329102 19.567099 13207.936523", "Svetloyarsk [N3-9]"));
        m_EventLocations.Insert(new BattlegroundsLocation("13602.665039 13.117899 14023.579102", "Turovo [N2-8]"));
        m_EventLocations.Insert(new BattlegroundsLocation("12861.069336 19.204769 14172.349609", "Northeast Highway [M2-9]"));
        m_EventLocations.Insert(new BattlegroundsLocation("12160.919922 27.402435 14331.293945", "Novodmitrovsk East [M2-7]"));
        m_EventLocations.Insert(new BattlegroundsLocation("11413.127930 78.545227 14590.172852", "Novodmitrovsk Hospital [L1-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("10720.474609 53.749374 14348.192383", "Novodmitrovsk West [K2-9]"));
        m_EventLocations.Insert(new BattlegroundsLocation("10083.195313 62.950165 13949.175781", "Pobeda Dam [K2-4]"));
        m_EventLocations.Insert(new BattlegroundsLocation("9283.264648 107.195251 13492.719727", "Klyuch Castle [J2-1]"));
        m_EventLocations.Insert(new BattlegroundsLocation("8456.462891 205.138306 13981.750000", "Arsenovo [I2-5]"));
        m_EventLocations.Insert(new BattlegroundsLocation("7864.786621 343.103027 14694.590820", "Troitskoye [H1-6]"));
        m_EventLocations.Insert(new BattlegroundsLocation("7086.377441 355.552399 14582.300781", "Kamensk Mine [H1-1]"));
        m_EventLocations.Insert(new BattlegroundsLocation("6485.212891 463.612457 14855.518555", "Krasnoye Radio Tower [G1-5]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5894.878418 478.341888 14761.952148", "Skalka Cliffside [F1-6]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4918.613770 450.177795 14855.055664", "Stary Yar Lodge [E1-6]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4316.005371 341.312714 14463.483398", "Cliffside Rally [E1-1]"));
        m_EventLocations.Insert(new BattlegroundsLocation("3677.458740 363.582947 14818.383789", "Pobeda [D1-6]"));
        m_EventLocations.Insert(new BattlegroundsLocation("3334.371338 324.772430 15176.308594", "Tisy Checkpoint [D1-8]"));
        m_EventLocations.Insert(new BattlegroundsLocation("2588.934570 448.003601 14840.119141", "Tisy Lodge [C1-5]"));*/

        // NOVAYA > INLAND
        /*m_EventLocations.Insert(new BattlegroundsLocation("4328.193359 193.591904 13641.657227", "Novaya Backroads [E2-1]"));
        m_EventLocations.Insert(new BattlegroundsLocation("3655.429688 192.616440 13097.946289", "Novaya Church [D3-8]"));
        m_EventLocations.Insert(new BattlegroundsLocation("3532.126709 219.861252 12559.313477", "Novaya Train Station [D3-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4033.652832 375.948212 11731.096680", "Bashnya Headquarters [E4-4]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4195.347656 338.409973 11045.544922", "NW Airfield Tents [E5-7]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4641.945801 339.000000 10465.122070", "NW Airfield ATC [E5-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5366.830566 332.787781 9908.400391", "NW Airfield Boot Camp [F6-5]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4527.834473 338.890076 9374.999023", "NW Airfield Gate [E6-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("3799.243164 310.847321 8845.937500", "Vybor [D7-6]"));
        m_EventLocations.Insert(new BattlegroundsLocation("4524.168945 316.188538 8276.397461", "Vybor Military [E8-8]"));

        // OLSHA > INLAND
        m_EventLocations.Insert(new BattlegroundsLocation("12343.433594 140.012405 12679.003906", "Krasnostav Airfield [M3-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("11865.374023 140.012375 12454.171875", "Krasnostav Airfield [L3-3]"));
        m_EventLocations.Insert(new BattlegroundsLocation("11145.664063 198.719849 12262.441406", "Krasnostav Downtown [L4-7]"));
        m_EventLocations.Insert(new BattlegroundsLocation("10267.595703 367.747223 12056.404297", "Zolotar Castle [K4-7]"));
        m_EventLocations.Insert(new BattlegroundsLocation("9558.753906 242.668304 11815.359375", "Novy Lug Checkpoint [J4-5]"));
        m_EventLocations.Insert(new BattlegroundsLocation("8839.286133 221.401810 11658.976563", "Gvozdno Farm [I4-3]"));
        m_EventLocations.Insert(new BattlegroundsLocation("8151.977539 140.416183 12394.154297", "Severograd Train Bridge [I3-1]"));
        m_EventLocations.Insert(new BattlegroundsLocation("8024.207520 113.893333 12763.243164", "Severograd Hospital [I3-4]"));
        m_EventLocations.Insert(new BattlegroundsLocation("7084.227051 130.547287 12708.708984", "Severograd Checkpoint [H3-4]"));

        // SOLNECHNY > INLAND
        m_EventLocations.Insert(new BattlegroundsLocation("12265.712891 34.332432 6249.709473", "Solnechny Valley [M10-7]"));
        m_EventLocations.Insert(new BattlegroundsLocation("11546.066406 69.664169 6614.298828", "Dolina East [L9-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("11177.751953 76.212982 6580.968750", "Dolina PD [L9-1]"));
        m_EventLocations.Insert(new BattlegroundsLocation("10494.127930 114.065918 7033.791992", "Dolina Valley [K9-8]"));
        m_EventLocations.Insert(new BattlegroundsLocation("10655.436523 210.908066 8048.149902", "Polyana [K8-8]"));
        m_EventLocations.Insert(new BattlegroundsLocation("10436.261719 223.080002 8888.083008", "Gorka Gas Station [K7-5]"));
        m_EventLocations.Insert(new BattlegroundsLocation("9503.199219 304.010010 8830.609375", "Gorka [J7-5]"));
        m_EventLocations.Insert(new BattlegroundsLocation("8805.537109 368.743683 9004.934570", "Crossroads [I7-6]"));
        m_EventLocations.Insert(new BattlegroundsLocation("8083.964844 458.862427 9319.800781", "Radio Zenit [I7-7]"));

        // CHERNO > INLAND
        m_EventLocations.Insert(new BattlegroundsLocation("6867.632324 58.721142 3672.831299", "Dubovo [G12-3]"));
        m_EventLocations.Insert(new BattlegroundsLocation("7070.936523 192.397263 4330.884766", "Builder Hunting Camp [H12-7]"));
        m_EventLocations.Insert(new BattlegroundsLocation("7423.164551 217.174911 5161.897949", "Mogilevka Church [H11-8]"));
        m_EventLocations.Insert(new BattlegroundsLocation("6562.314453 392.796844 5597.143555", "Zub Castle [G10-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("6451.001953 382.903168 6522.003418", "Vyshnoye Lookout [G9-2]"));
        m_EventLocations.Insert(new BattlegroundsLocation("7238.611328 253.446732 7027.446777", "Nadezhda [H9-7]"));
        m_EventLocations.Insert(new BattlegroundsLocation("7184.218262 292.885773 7667.784180", "Novy Sobor [H8-1]"));
        m_EventLocations.Insert(new BattlegroundsLocation("6349.589844 304.973999 7792.907715", "Stary Sobor Military [G8-5]"));
        m_EventLocations.Insert(new BattlegroundsLocation("5970.486816 302.240875 7881.390137", "Stary Sobor West [F8-6]"));

        // MYSHKINO > INLAND
        m_EventLocations.Insert(new BattlegroundsLocation("2009.832764 253.958710 7343.135254", "Myshkino [C9-7]"));*/