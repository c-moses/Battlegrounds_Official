
modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);
		actions.Insert(ActionTeleport);
        actions.Insert(ActionFollowTeleport);
	}
};

class ActionFollowTeleportCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT);
	}
};

// =================================================================

class ActionFollowTeleport : ActionContinuousBase
{
	void ActionFollowTeleport()
	{
		m_CallbackClass = ActionFollowTeleportCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CHECKPULSE;
		m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
		m_Text = "Follow Teleport";
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        PlayerBase ntarget = PlayerBase.Cast(target.GetObject());
        if (ntarget && ntarget.IsAlive() && ntarget.CanBeFollowed())
        {
            EntityAI playerArmband = PlayerHelper.GetTeamArmband(player);
            EntityAI targetArmband = PlayerHelper.GetTeamArmband(ntarget);

            if (playerArmband && targetArmband && playerArmband.GetType() == targetArmband.GetType())
                return true;
        }
        return false;
    }

	override void OnFinishProgressServer(ActionData action_data)
	{
		PlayerBase target_player = PlayerBase.Cast(action_data.m_Target.GetObject());
		if (target_player)
		{
            PlayerBase player = action_data.m_Player;
            target_player.StartFollow(player);
            player.SetFollowing(true);

            string notifyFollowerName = NOTIFY_FOLLOWER + player.GetName();
            string notifyFollowedName = NOTIFY_FOLLOWED + target_player.GetName();
            NotificationSystem.SendNotificationToPlayerExtended(player, NOTIFY_LENGTH, notifyFollowerName, NOTIFY_FOLLOW_MSG, ICON_FOLLOW);
            NotificationSystem.SendNotificationToPlayerExtended(target_player, NOTIFY_LENGTH, notifyFollowedName, NOTIFY_FOLLOW_MSG, ICON_FOLLOW);
		}
	}

    // ============================
	
	/*override void OnFinishProgressClient(ActionData action_data)
	{
		PlayerBase ntarget = PlayerBase.Cast(action_data.m_Target.GetObject());
		if(ntarget)
		{ }
	}*/
}

// =============================================================

class ActionTeleport : ActionOpenDoors
{
    protected BattlegroundsTeleporterPanel_Base m_Teleporter;

    override string GetText()
    {
        if (m_Teleporter)
        {
            return "Teleport";
        }
        return "Teleport";
    }

    protected bool IsTeleporterPanel(Object target) 
    {
        return target.IsInherited(BattlegroundsTeleporterPanel_Base);
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (target.GetObject().IsInherited(Building) && !IsTeleporterPanel(target.GetObject()))
            return false;

        if (IsTeleporterPanel(target.GetObject()))
        {
            m_Teleporter = BattlegroundsTeleporterPanel_Base.Cast(target.GetObject());
            return true;
        }

        return super.ActionCondition(player, target, item);
    }

    override void OnStartServer(ActionData action_data)
    {
        BattlegroundsTeleporterPanel_Base teleporter = BattlegroundsTeleporterPanel_Base.Cast(action_data.m_Target.GetObject());

        if (teleporter)
        {
            vector safeTeleportPosition = teleporter.GetTeleportDestination(action_data.m_Player);
            action_data.m_Player.SetPosition(safeTeleportPosition);
            PlayerBase targetPlayer = action_data.m_Player;

            if (targetPlayer.GetSafezoneStatus() == SPAWN_PROTECTION)
                targetPlayer.SetSafezoneStatus(NO_ZONE);

            if (targetPlayer.IsBeingFollowed())
            {
                HandleFollowers(safeTeleportPosition, targetPlayer.GetFollowerIds());
            }

            teleporter.Close(0);
        }
        else
        {
            super.OnStartServer(action_data);
        }
    }

    void HandleFollowers(vector pos, array<string> followerIDs)
    {
        foreach (string followerId : followerIDs)
        {
            PlayerIdentity followerIdentity = GetIdentityById(followerId);
            if (followerIdentity)
            {
                PlayerBase followerPlayer = PlayerHelper.GetPlayerByIdentity(followerIdentity);
                if (followerPlayer)
                {
                    followerPlayer.SetPosition(pos);
                    if (followerPlayer.GetSafezoneStatus() == SPAWN_PROTECTION)
                        followerPlayer.SetSafezoneStatus(NO_ZONE);
                }
            }
        }
    }

    PlayerIdentity GetIdentityById(string id)
    {
        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);

        foreach (Man player : players)
        {
            PlayerIdentity identity = player.GetIdentity();
            if (identity && identity.GetId() == id)
            {
                return identity;
            }
        }

        return null;
    }
}