class BGFireworks extends FireworksLauncher
{
	void BGFireworks()
	{
		if (GetGame().IsServer())
			SetAllowDamage(false);
	}

    override bool CanPutInCargo( EntityAI parent )
	{ return false; }
	
	override bool CanPutIntoHands( EntityAI parent )
	{ return false; }

    override bool DisableVicinityIcon()
    { return true; }
}

class BGSmokeGreen extends M18SmokeGrenade_Green
{
    void BGSmokeGreen()
    {
        if (GetGame())
        {
            if (GetGame().IsServer())
            { 
				SetSmokeGrenadeState(ESmokeGrenadeState.LOOP);
				SetAllowDamage(false);
			}
        }
        else
        { 
			SetSmokeGrenadeState(ESmokeGrenadeState.NO_SMOKE); 
		}
    }

    override bool CanPutInCargo( EntityAI parent )
	{ return false; }
	
	override bool CanPutIntoHands( EntityAI parent )
	{ return false; }

    override bool DisableVicinityIcon()
    { return true; }
};

class BGSmokeRed extends M18SmokeGrenade_Red
{
    void BGSmokeRed()
    {
        if (GetGame())
        {
            if (GetGame().IsServer())
            { 
				SetSmokeGrenadeState(ESmokeGrenadeState.LOOP);
				SetAllowDamage(false);
			}
        }
        else
        { 
			SetSmokeGrenadeState(ESmokeGrenadeState.NO_SMOKE); 
		}
    }

    override bool CanPutInCargo( EntityAI parent )
	{ return false; }
	
	override bool CanPutIntoHands( EntityAI parent )
	{ return false; }

    override bool DisableVicinityIcon()
    { return true; }
};

class BattlegroundsRewardCrate extends DeployableContainer_Base
{
	void BattlegroundsRewardCrate()
	{
		if (GetGame().IsServer())
			SetAllowDamage(false);
	}

    override bool CanPutInCargo( EntityAI parent )
	{ return false; }
	
	override bool CanPutIntoHands( EntityAI parent )
	{ return false; }

    override bool DisableVicinityIcon()
    { return true; }

	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		PlayerBase new_player = null;
		PlayerBase old_player = null;
		
		if (newLoc.GetParent())
			new_player = PlayerBase.Cast(newLoc.GetParent().GetHierarchyRootPlayer());
		
		if (oldLoc.GetParent())
			old_player = PlayerBase.Cast(oldLoc.GetParent().GetHierarchyRootPlayer());
		
		if (old_player && oldLoc.GetType() == InventoryLocationType.HANDS)
		{
			int r_index = old_player.GetHumanInventory().FindUserReservedLocationIndex(this);
			if (r_index >= 0)
			{
				InventoryLocation r_il = new InventoryLocation;
				old_player.GetHumanInventory().GetUserReservedLocation(r_index,r_il);
				old_player.GetHumanInventory().ClearUserReservedLocationAtIndex(r_index);
				int r_type = r_il.GetType();
				if (r_type == InventoryLocationType.CARGO || r_type == InventoryLocationType.PROXYCARGO)
				{ r_il.GetParent().GetOnReleaseLock().Invoke(this); }
				else if (r_type == InventoryLocationType.ATTACHMENT)
				{ r_il.GetParent().GetOnAttachmentReleaseLock().Invoke(this, r_il.GetSlot()); }
			}
		}
		
		if (newLoc.GetType() == InventoryLocationType.HANDS)
		{
			if (new_player)
				new_player.ForceStandUpForHeavyItems(newLoc.GetItem());

			if (m_OldLocation)
			{ m_OldLocation.Reset(); }
			GetGame().GetAnalyticsClient().OnItemAttachedAtPlayer(this,"Hands");    
		}
		else
		{
			if (new_player)
			{
				int res_index = new_player.GetHumanInventory().FindCollidingUserReservedLocationIndex(this, newLoc);
				if (res_index >= 0)
				{
					InventoryLocation il = new InventoryLocation;
					new_player.GetHumanInventory().GetUserReservedLocation(res_index,il);
					ItemBase it = ItemBase.Cast(il.GetItem());
					new_player.GetHumanInventory().ClearUserReservedLocationAtIndex(res_index);
					int rel_type = il.GetType();
					if (rel_type == InventoryLocationType.CARGO || rel_type == InventoryLocationType.PROXYCARGO)
					{ il.GetParent().GetOnReleaseLock().Invoke(it); }
					else if (rel_type == InventoryLocationType.ATTACHMENT)
					{ il.GetParent().GetOnAttachmentReleaseLock().Invoke(it, il.GetSlot()); }
				}
			}
			else if (old_player && newLoc.GetType() == InventoryLocationType.GROUND && m_ThrowItemOnDrop)
			{ m_ThrowItemOnDrop = false; }
		
			if (m_OldLocation)
			{ m_OldLocation.Reset(); }
		}
	}
};