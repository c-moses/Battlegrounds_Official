modded class ModItemRegisterCallbacks
{
	override void RegisterOneHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
    {
        super.RegisterOneHanded(pType, pBehavior);
		pType.AddItemInHandsProfileIK("UniversalSuppressorM4", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/attachments/muzzle/suppressor_556.anm");
		pType.AddItemInHandsProfileIK("UniversalSuppressorAK", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/attachments/muzzle/suppressor_556.anm");
		pType.AddItemInHandsProfileIK("FirstAidKit_BG", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior,"dz/anims/anm/player/ik/gear/first_aid_kit.anm");
		pType.AddItemInHandsProfileIK("SnD_Bomb", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/explosives/ImprovisedExplosive.anm");
	}
};

modded class DayZPlayerImplement
{
	override void HandleView()
	{
		super.HandleView();
		HumanInputController hic = GetInputController();
		if (!m_MovementState.IsRaised() && hic.IsZoomToggle())        
		{ 
			m_CameraEyeZoomLevel = ECameraZoomType.NORMAL; 
		}
	}
}

modded class ActionConstructor 
{
    override void RegisterActions(TTypenameArray actions)
    {
		super.RegisterActions(actions);
		actions.Insert(ActionFirstAidTarget);
		actions.Insert(ActionFirstAidSelf);
	}
};

modded class ActionCraftArmband: ActionContinuousBase
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		return false;
	}
}

modded class ActionConsume: ActionContinuousBase
{
	override bool CanBeUsedInVehicle()
	{ 
		return true; 
	}
}

modded class EmoteSuicide extends EmoteBase
{
	override bool EmoteCondition(int stancemask)
	{
		if (m_Player.GetSafezoneStatus() == SZ_INSIDE || m_Player.GetSafezoneStatus() == SZ_LEAVING || m_Player.GetSafezoneStatus() == SPAWN_PROTECTION)
		{
			return false;
		}
		
		return super.EmoteCondition(stancemask);
	}
}

modded class ActionRestrainTarget
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if(player.GetSafezoneStatus() == SZ_INSIDE || player.GetSafezoneStatus() == SZ_LEAVING || player.GetSafezoneStatus() == SPAWN_PROTECTION)
            return false;

        return super.ActionCondition(player, target, item);
    }
}

modded class ActionRestrainSelf
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if(player.GetSafezoneStatus() == SZ_INSIDE || player.GetSafezoneStatus() == SZ_LEAVING || player.GetSafezoneStatus() == SPAWN_PROTECTION)
            return false;

        return super.ActionCondition(player, target, item);
    }
}

modded class ActionForceFeed: ActionForceConsume
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if(player.GetSafezoneStatus() == SZ_INSIDE || player.GetSafezoneStatus() == SZ_LEAVING || player.GetSafezoneStatus() == SPAWN_PROTECTION)
			return false;
		
		return super.ActionCondition(player, target, item);
	}
}

modded class ActionForceDrink: ActionForceConsume
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (player.GetSafezoneStatus() == SZ_INSIDE || player.GetSafezoneStatus() == SZ_LEAVING || player.GetSafezoneStatus() == SPAWN_PROTECTION)
			return false;

		return super.ActionCondition(player, target, item);
	}
}

modded class ActionForceFeedSmall: ActionForceConsume
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (player.GetSafezoneStatus() == SZ_INSIDE || player.GetSafezoneStatus() == SZ_LEAVING || player.GetSafezoneStatus() == SPAWN_PROTECTION)
			return false;

		return super.ActionCondition(player, target, item);
	}
}

// ========================= 

modded class ItemBase
{ 
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef) {} 
	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		EntityAI rootEntity = GetHierarchyRootPlayer();
		PlayerBase targetPlayer = PlayerBase.Cast(rootEntity);
		if (targetPlayer && (targetPlayer.GetSafezoneStatus() == SZ_INSIDE || targetPlayer.GetSafezoneStatus() == SZ_LEAVING || targetPlayer.GetSafezoneStatus() == SPAWN_PROTECTION))
		{
			return false;
		}

		PlayerBase sourcePlayer = null;
		if (source && source.GetHierarchyRootPlayer())
		{
			sourcePlayer = PlayerBase.Cast(source.GetHierarchyRootPlayer());
		}

		if (sourcePlayer && (sourcePlayer.GetSafezoneStatus() == SZ_INSIDE || sourcePlayer.GetSafezoneStatus() == SZ_LEAVING || sourcePlayer.GetSafezoneStatus() == SPAWN_PROTECTION))
		{
			return false;
		}

		if (sourcePlayer && targetPlayer && sourcePlayer.HasArmband() && targetPlayer.HasArmband())
		{
			if (sourcePlayer.GetArmband() == targetPlayer.GetArmband())
			{
				return false;
			}
		}

		return true;
	}
}