class SafezoneHandler
{
    ref SafezoneLocations locations;
    ref SafezoneLocation currentLocation;
    private bool hasStarted = false;
    private int zoneStatus = SPAWN_PROTECTION;
    private bool isCooldownActive = false;
    PlayerBase player;

    void SafezoneHandler() {}

    bool HasStarted() 
    {
        return hasStarted; 
    }

    int GetZoneStatus()
    {
        return zoneStatus;
    }

    PlayerBase GetPlayer()
    {
        if (!player)
            player = PlayerBase.Cast(GetGame().GetPlayer());
            
        return player;
    }

    void StartSZCheck(SafezoneLocations szlocations)
    {
        this.locations = szlocations;
        player = GetPlayer();

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Update, 1000, true);
        zoneStatus = SPAWN_PROTECTION; // new
        hasStarted = true;
    }

    void StopSZCheck()
    {
        currentLocation = null;
        zoneStatus = NO_ZONE;
        isCooldownActive = false;
        GetRPCManager().SendRPC("Battlegrounds", "SafezoneStatus", new Param1<int>(zoneStatus), true, null);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.Update);
        hasStarted = false;
    }

    void Update()
    {
        CheckSafezoneLocations();
    }

    void CheckSafezoneLocations()
    {
        foreach(SafezoneLocation location: locations.locations)
        {
            CheckSafezoneLocation(location);
        }
    }

    void CheckSafezoneLocation(SafezoneLocation location)
    {
        bool isCurrentlyInZone = IsInRadius(location.position, location.radius);

        if (zoneStatus == SPAWN_PROTECTION)
        {
            if (isCurrentlyInZone)
            {
                currentLocation = location;
                OnEnterZone(location);
            }
            else
            {
                zoneStatus = NO_ZONE;
                GetRPCManager().SendRPC("Battlegrounds", "SafezoneStatus", new Param1<int>(zoneStatus), true, null);
            }
            return;
        }

        if (isCurrentlyInZone)
        {
            if (isCooldownActive && (location.id == 0 || location.id > 0))
            {
                EnableExitCooldown(false);
                currentLocation = location;
                OnEnterZone(location);
            }
            else if (location.id == 0 && zoneStatus == SZ_INSIDE && currentLocation == location)
            {
                if (isCooldownActive)
                {
                    EnableExitCooldown(false);
                }
            }
            else if (zoneStatus == NO_ZONE || currentLocation != location)
            {
                currentLocation = location;
                OnEnterZone(location);
            }
        }
        else if (zoneStatus != NO_ZONE && !isCurrentlyInZone && currentLocation == location)
        {
            OnExitZone(zoneStatus);
        }
    }

    bool IsInRadius(vector position, int radius)
    {
        if (!player)
        {
            GetPlayer();
        }

        if (!player || !player.IsAlive() || player.IsPlayerDisconnected())
        {
            return false;
        }

        vector playerPos = Vector(player.GetPosition()[0], 0, player.GetPosition()[2]);
        float distance = vector.Distance(playerPos, position);
        
        return distance <= radius;
    }

    void OnEnterZone(SafezoneLocation location)
    {
        if (location.id == 0)
        {
            zoneStatus = SZ_INSIDE;
            NotificationSystem.AddNotificationExtended(NOTIFY_LENGTH, NOTIFY_SZ_ENTERED, "", ICON_SZ);
        }
        else if (location.id > 0)
        {
            zoneStatus = BZ_INSIDE;
            NotificationSystem.AddNotificationExtended(NOTIFY_LENGTH, NOTIFY_BZ_ENTERED, "", ICON_BZ);
        }

        GetRPCManager().SendRPC("Battlegrounds", "SafezoneStatus", new Param1<int> (zoneStatus), true, null);
    }

    void OnExitZone(int exitZoneStatus)
    {
        if (exitZoneStatus == SZ_INSIDE)
        {
            EnableExitCooldown(true);
        }
        else if (exitZoneStatus == BZ_INSIDE)
        {
            NotificationSystem.AddNotificationExtended(NOTIFY_LENGTH, NOTIFY_BZ_EXITED, "", ICON_BZ);
            currentLocation = null;
            zoneStatus = NO_ZONE;
            GetRPCManager().SendRPC("Battlegrounds", "SafezoneStatus", new Param1<int>(zoneStatus), true, null);
        }
    }

    void EnableExitCooldown(bool enable)
    {
        if (enable)
        {
            if (isCooldownActive)
                return;

            isCooldownActive = true;
            zoneStatus = SZ_LEAVING;
            GetRPCManager().SendRPC("Battlegrounds", "SafezoneStatus", new Param1<int>(zoneStatus), true, null);
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnCooldownComplete, 10000, false);
        }
        else
        {
            if (isCooldownActive)
            {
                GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(OnCooldownComplete);
                isCooldownActive = false;
                zoneStatus = SZ_INSIDE;
                GetRPCManager().SendRPC("Battlegrounds", "SafezoneStatus", new Param1<int>(zoneStatus), true, null);
            }
        }
    }

    void OnCooldownComplete()
    {
        NotificationSystem.AddNotificationExtended(NOTIFY_LENGTH, NOTIFY_SZ_EXITED, "", ICON_BZ);
        currentLocation = null;
        zoneStatus = NO_ZONE;
        GetRPCManager().SendRPC("Battlegrounds", "SafezoneStatus", new Param1<int>(zoneStatus), true, null);
        isCooldownActive = false;
    }
}