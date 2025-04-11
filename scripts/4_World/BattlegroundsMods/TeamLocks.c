modded class ActionOpenFence: ActionInteractBase
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        Object targetObject = target.GetObject();
        if (targetObject && targetObject.CanUseConstruction())
        {
            Fence fence = Fence.Cast(targetObject);
            if (fence)
            {
                if (!fence.CanOpenFence())
                { return false; }

                EntityAI targetEntity = EntityAI.Cast(targetObject);
                if (targetEntity)
                {
                    GreenTeamLock greenlock = GreenTeamLock.Cast(targetEntity.FindAttachmentBySlotName("BattlegroundsLock"));
                    if (greenlock)
                    {
                        if (player.GetArmband() == TEAM_GREEN)
                            return true; 
                    }

                    BlueTeamLock bluelock = BlueTeamLock.Cast(targetEntity.FindAttachmentBySlotName("BattlegroundsLock"));
                    if (bluelock)
                    {
                        if (player.GetArmband() == TEAM_BLUE)
                            return true; 
                    }

                    RedTeamLock redlock = RedTeamLock.Cast(targetEntity.FindAttachmentBySlotName("BattlegroundsLock"));
                    if (redlock)
                    {
                        if (player.GetArmband() == TEAM_RED)
                            return true; 
                    }

                    OrangeTeamLock orangelock = OrangeTeamLock.Cast(targetEntity.FindAttachmentBySlotName("BattlegroundsLock"));
                    if (orangelock)
                    {
                        if (player.GetArmband() == TEAM_ORANGE)
                            return true; 
                    }

                    return super.ActionCondition(player, target, item);
                }
            }
        }
        return false; 
    }
}

modded class ActionCloseFence: ActionInteractBase
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        Object targetObject = target.GetObject();
        if (targetObject && targetObject.CanUseConstruction())
        {
            Fence fence = Fence.Cast(targetObject);

            if (fence)
            {
                if (!fence.CanCloseFence())
                { return false; }

                EntityAI targetEntity = EntityAI.Cast(targetObject);
                if (targetEntity)
                {
                    GreenTeamLock greenlock = GreenTeamLock.Cast(targetEntity.FindAttachmentBySlotName("BattlegroundsLock"));
                    if (greenlock)
                    {
                        if (player.GetArmband() == TEAM_GREEN)
                            return true; 
                    }

                    BlueTeamLock bluelock = BlueTeamLock.Cast(targetEntity.FindAttachmentBySlotName("BattlegroundsLock"));
                    if (bluelock)
                    {
                        if (player.GetArmband() == TEAM_BLUE)
                            return true; 
                    }

                    RedTeamLock redlock = RedTeamLock.Cast(targetEntity.FindAttachmentBySlotName("BattlegroundsLock"));
                    if (redlock)
                    {
                        if (player.GetArmband() == TEAM_RED)
                            return true; 
                    }

                    OrangeTeamLock orangelock = OrangeTeamLock.Cast(targetEntity.FindAttachmentBySlotName("BattlegroundsLock"));
                    if (orangelock)
                    {
                        if (player.GetArmband() == TEAM_ORANGE)
                            return true; 
                    }
                    return super.ActionCondition(player, target, item);
                }
            }
        }
        return false; 
    }
}

class BattlegroundsLock extends Inventory_Base {}
class GreenTeamLock extends BattlegroundsLock {}
class BlueTeamLock extends BattlegroundsLock {}
class RedTeamLock extends BattlegroundsLock {}
class OrangeTeamLock extends BattlegroundsLock {}