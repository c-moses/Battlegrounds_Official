modded class ZombieBase extends DayZInfected
{
    private ref BattlegroundsRewardManager m_RewardManager;
	bool m_IsThrowingRock

////////////////////////////////////////

    void ZombieBase() 
    { 
		m_RewardManager = BattlegroundsRewardManager.GetInstance(); 
	}

	override void Init()
    {
        super.Init();
        CheckRockThrow();
    }

//////////////////////////////////////// conditions

    void CheckRockThrow()
    {
        if (IsAlive() && !IsCrawling())
        {
            if (m_ActualTarget && GetMindStateSynced() >= 2)
            {
                vector targetPosition = m_ActualTarget.GetPosition();
                vector zombiePosition = this.GetPosition();
                float distanceSq = vector.DistanceSq(targetPosition, zombiePosition);
                if (distanceSq > 4 && distanceSq < 100)
                {
                    vector zombiePositionAdjust = zombiePosition;
                    zombiePositionAdjust[1] = zombiePositionAdjust[1] + 0.3;
                    
                    vector targetPositionAdjust = targetPosition;
                    targetPositionAdjust[1] = targetPositionAdjust[1] + 0.7;

                    if (!RaycastPlayer(zombiePositionAdjust, targetPositionAdjust) && IsFacingTarget(targetPosition, zombiePosition, distanceSq, 20.0) && IsPlayerOnObstacle(targetPosition, zombiePosition))
                    {
                        HandleRockThrow();
                    }
                }
            }
            
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckRockThrow, 4 * 1000 + Math.RandomFloatInclusive(0, 1000), false);
        }
    }

    bool RaycastPlayer(vector zombiePositionAdjust, vector targetPositionAdjust)
    {
        bool hit;
		vector decalZombieHeight = "0 1.5 0";
		vector decalPlayerHeight = "0 1.0 0";
		zombiePositionAdjust += decalZombieHeight;
		targetPositionAdjust += decalPlayerHeight;
		
		float hitFraction;
		vector hitPosition, hitNormal;
		Object hitObject;
		int contactComponent;
		
		PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.BUILDING|PhxInteractionLayers.DOOR|PhxInteractionLayers.VEHICLE|PhxInteractionLayers.ITEM_LARGE|PhxInteractionLayers.FENCE;
		hit = DayZPhysics.RayCastBullet(zombiePositionAdjust, targetPositionAdjust, collisionLayerMask, null, hitObject, hitPosition, hitNormal, hitFraction);

		return hit;
    }

	bool IsFacingTarget(vector targetPosition, vector zombiePosition, float distanceSq, float maxDistance)
	{
		float maxThrowDistance = maxDistance * maxDistance;
		if (distanceSq < maxThrowDistance)
		{
			vector zombieDirection = this.GetDirection();
			vector throwRange = zombiePosition + zombieDirection * maxDistance;
			float distFrontOfZombieToPlayerSq = vector.DistanceSq(throwRange, targetPosition);

			if (distFrontOfZombieToPlayerSq < maxThrowDistance)
			    return true;
		}

		return false;
	}

	bool IsPlayerOnObstacle(vector targetPosition, vector zombiePosition)
	{
		if (targetPosition[1] > zombiePosition[1])
		{
			float targetGroundHeight =  GetGame().SurfaceY(targetPosition[0], targetPosition[2]);
			if (targetPosition[1] > targetGroundHeight + 0.8)
			    return true;
		}

		return false;
	}

//////////////////////////////////////// conditions met

    void HandleRockThrow()
    {
        m_IsThrowingRock = true;
        TriggerThrowAnimation();
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SpawnRock, 500, false);
    }

    void TriggerThrowAnimation()
    {
        int attackType = Math.RandomIntInclusive(0, 1);
        m_ActualAttackType = GetDayZInfectedType().ChooseAttack(DayZInfectedAttackGroupType.FIGHT, 0, 0);
        DayZInfectedCommandAttack dzInfectedCommandAttack = StartCommand_Attack(NULL, attackType, 1);
    }

    void SpawnRock()
    {
        vector zombiePosition = this.GetPosition();
        vector zombieDirection = this.GetDirection();

        zombiePosition[1] = zombiePosition[1] + 1.8;
        zombiePosition = zombiePosition + (zombieDirection * 0.2);

        EntityAI rockToThrow = EntityAI.Cast(GetGame().CreateObject("Rock", zombiePosition, false, false));
        Rock theRock = Rock.Cast(rockToThrow);
        if (theRock)
            theRock.IsRockThrown = true;

        if (rockToThrow)
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ThrowRock, 100, false, rockToThrow);
    }

    void ThrowRock(EntityAI theRock)
    {
        vector force;
        if (m_ActualTarget)
        {
            vector targetPos = m_ActualTarget.GetPosition() + "0 1.6 0";
            force = ((targetPos - theRock.GetPosition()).Normalized()) * 450;
        }

        force[1] = force[1] + 450 * 0.04;
		InventoryItem theRockInv = InventoryItem.Cast(theRock);
		if (theRockInv)
		    theRockInv.ThrowPhysically(NULL, force);

        m_IsThrowingRock = false;
    }

////////////////////////////////////////

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		PlayerBase playerKiller = PlayerBase.Cast(killer);
		if (!playerKiller)
		{
			if (killer.IsInherited(Weapon_Base))
			{
				playerKiller = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
			}
			else if (killer.IsInherited(ItemBase))
			{
				playerKiller = PlayerBase.Cast(EntityAI.Cast(killer).GetHierarchyParent());
			}
		}

		if (playerKiller)
		{
			int currencyAmount = 50;
			PlayerIdentity killerID = playerKiller.GetIdentity();
			CurrencyHelper helper = CurrencyHelper.GetInstance();
			helper.UpdateCurrency(killerID.GetPlainId(), currencyAmount, killerID);
			m_RewardManager.UpdatePlayerStat(killerID, "cashEarned", currencyAmount);
			m_RewardManager.UpdatePlayerStat(killerID, "zombieKills", 1);
		}
	}
    
    bool IsTargetInSafeZone(EntityAI target)
    {
        PlayerBase player = PlayerBase.Cast(target);
        
        if (player && (player.GetSafezoneStatus() == SZ_INSIDE || player.GetSafezoneStatus() == SZ_LEAVING || player.GetSafezoneStatus() == SPAWN_PROTECTION))
        { 
			return true;
		}

        return false;
    }

    override bool ChaseAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
    {
        m_ActualTarget = pInputController.GetTargetEntity();

        if (IsTargetInSafeZone(m_ActualTarget))
            return false;

        return super.ChaseAttackLogic(pCurrentCommandID,pInputController,pDt);
    }

    override bool FightAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		m_ActualTarget = pInputController.GetTargetEntity();

        if (IsTargetInSafeZone(m_ActualTarget))
            return false;

		return super.FightAttackLogic(pCurrentCommandID,pInputController,pDt);
	}
}