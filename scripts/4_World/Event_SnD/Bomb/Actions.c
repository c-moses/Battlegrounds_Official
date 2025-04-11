modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
		super.RegisterActions(actions);
		actions.Insert(ActionArmBomb);
		actions.Insert(ActionDisarmBomb);
	}
};

class ActionSnDBombCB extends ActionContinuousBaseCB
{
    override void CreateActionComponent()
	{
		float Action_Time = 6.0;
		m_ActionData.m_ActionComponent = new CAContinuousTime(Action_Time);
	}
}

/////////////////// ARM BOMB ///////////////////

class ActionArmBomb extends ActionContinuousBase
{
    void ActionArmBomb()
    {
        m_CallbackClass = ActionSnDBombCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
        m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
        m_Text = "Arm Bomb";
    }

    override void CreateConditionComponents()  
    {
        m_ConditionItem = new CCINonRuined;
        m_ConditionTarget = new CCTCursor;
    }

    override typename GetInputType()
    {
        return ContinuousInteractActionInput;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        Object target_O = target.GetObject();
        
        if (item && item.IsKindOf("SnD_Bomb") && target_O && target_O.IsInherited(SnD_Site))
        {
            SnD_Bomb bomb = SnD_Bomb.Cast(item);
            if (bomb && bomb.GetBombState() == -1)
            {
                return true;
            }
        }
        
        return false;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        SnD_Bomb bomb = SnD_Bomb.Cast(action_data.m_MainItem);
        if (bomb)
        {
            action_data.m_Player.ServerDropEntity(bomb);
            bomb.SetBombState(0);

            string armband = action_data.m_Player.GetArmband();
            Object target = action_data.m_Target.GetObject();
            if (target && target.IsInherited(SnD_Site))
                string site = target.GetType();

            if (armband)
                bomb.OnArmed(armband, site);
        }
    }
}

/////////////////// DISARM BOMB ///////////////////

class ActionDisarmBomb extends ActionContinuousBase
{
    void ActionDisarmBomb()
    {
        m_CallbackClass = ActionSnDBombCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
        m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
        m_Text = "Disarm Bomb";
    }

    override void CreateConditionComponents()  
    {
        m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINotPresent;
    }

    override typename GetInputType()
    {
        return ContinuousInteractActionInput;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        Object target_O = target.GetObject();

        if (target_O && target_O.IsKindOf("SnD_Bomb"))
        {
            SnD_Bomb bomb = SnD_Bomb.Cast(target_O);
            if (bomb && bomb.GetBombState() == 0)
            {
                string playerArmband = player.GetArmband();
                if (playerArmband == bomb.ArmedBy())
                {
                    return false;
                }

                return true;
            }
        }

        return false;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        SnD_Bomb bomb = SnD_Bomb.Cast(action_data.m_Target.GetObject());
        if (bomb)
        {
            action_data.m_Player.ServerDropEntity(bomb);
            bomb.SetBombState(1);

            string armband = action_data.m_Player.GetArmband();
            if (armband)
            {
                bomb.OnDisarmed(armband);
            }
        }
    }
}