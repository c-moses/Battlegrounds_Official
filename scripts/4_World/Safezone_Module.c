[CF_RegisterModule(SafezoneModule)]
class SafezoneModule: CF_ModuleWorld
{
    ref SafezoneLocations locations;
    ref SafezoneHandler safezoneHandler;

    override void OnInit()
    {
        super.OnInit();
        EnableMissionStart();
        EnableInvokeConnect();
    }

    void OnMissionStartHandler()
    {
        if(GetGame().IsServer()) 
        {
            locations = SafezoneLocations.GetInstance();
        }
        else
        {
            locations = SafezoneLocations.GetInstance();
            safezoneHandler = new SafezoneHandler();
        }

        InitRPCs();
    }

    void InitRPCs()
    {
        if(GetGame().IsServer()) 
        {
            GetRPCManager().AddRPC("Battlegrounds", "SafezoneStatus", this, SingleplayerExecutionType.Server);
        }
        else
        {
            GetRPCManager().AddRPC("Battlegrounds", "SendSafezones", this, SingleplayerExecutionType.Client);
        }
    }

// ===============================================================================

    void SafezoneStatus(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type != CallType.Server)
            return;

        Param1<int> data;
        if(!ctx.Read(data))
            return;

        int status = data.param1;

        PlayerBase player = GetPlayerByIdentity(sender);
        if(player)
        {
            player.SetSafezoneStatus(status);
        }
    }

    PlayerBase GetPlayerByIdentity(PlayerIdentity sender)
  	{
  		int	low	 =	0;
  		int	high =	0;
  		GetGame().GetPlayerNetworkIDByIdentityID( sender.GetPlayerId(), low, high);
  		return PlayerBase.Cast(GetGame().GetObjectByNetworkId(low, high));
  	}

// ===============================================================================

    override void OnInvokeConnect(Class sender, CF_EventArgs args)
    {
        super.OnInvokeConnect(sender, args);

        auto cArgs = CF_EventPlayerArgs.Cast(args);

        if(!IsMissionHost())
            return;

        if(!cArgs.Player || !cArgs.Identity)
            return;

        RequestSafeZones(cArgs.Identity);
    }

    void RequestSafeZones(PlayerIdentity sender)
    {
        GetRPCManager().SendRPC("Battlegrounds", "SendSafezones", new Param1<SafezoneLocations>(locations), true, sender);
    }

    void SendSafezones(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type != CallType.Client)
            return;

        Param1<SafezoneLocations> data;
        if(!ctx.Read(data))
            return;

        locations = data.param1;

        if (locations)
        {
            if (!safezoneHandler.HasStarted())
            {
                safezoneHandler.StartSZCheck(locations);
            }
        }
        else
        {
            safezoneHandler.StopSZCheck();
        }
    }

    override void OnMissionStart(Class sender, CF_EventArgs args)
    {
        super.OnMissionStart(sender, args);
        OnMissionStartHandler();
    }
}