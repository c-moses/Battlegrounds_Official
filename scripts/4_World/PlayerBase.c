modded class PlayerBase extends ManBase
{
    static const int RPC_CHAT_CLIENT_SEND_GLOBAL = -42069;
    private ref BattlegroundsRewardManager m_RewardManager;
    private ref EventState m_EventState;
    private ref EventTeams m_EventTeams;
    private ref KillfeedWrapper m_Killfeed;
    private int netSync_safezoneStatus = SPAWN_PROTECTION;
    private string m_TeamID = NO_TEAM;
    private bool m_HasArmband = false;
    private bool IsFallDeath = false;

    protected ref array<string> m_FollowerIds = new array<string>();
    protected bool m_IsFollowing = false;
    protected bool m_IsFollowed = false;
    protected int m_FollowTime = 30;

    void PlayerBase()
    { 
        m_RewardManager = BattlegroundsRewardManager.GetInstance();
        m_EventState = EventState.GetInstance();
        m_EventTeams = EventTeams.GetInstance();

        if (GetGame().IsClient())
		    m_Killfeed = new KillfeedWrapper();
    }

    override void SetActions(out TInputActionMap InputActionMap)
	{
        super.SetActions(InputActionMap);
		AddAction(ActionTeleport, InputActionMap);
        AddAction(ActionFollowTeleport, InputActionMap);
        AddAction(ActionArmBomb, InputActionMap);
        AddAction(ActionDisarmBomb, InputActionMap);
    }

// ================================= ROCK THROW =============================================

    override void EOnContact(IEntity other, Contact extra)
    {
        Rock theRock = Rock.Cast(other);
        if (theRock && theRock.IsRockThrown == true)
        {
            theRock.IsRockThrown = false;
            HitByRock();

            EffectSound sound = SEffectManager.PlaySoundOnObject("BlockingAttack_SoundSet", this);
		    sound.SetAutodestroy(true);
        }

        super.EOnContact(other, extra);
    }

    void HitByRock()
    {
        ApplyDamage(1.5);
        ApplyDamage(30.0, "Shock");
    }

    void ApplyDamage(float dmg, string type = "")
    {
        float currentDmg = this.GetHealth("", type);
        float newDmg = currentDmg - 15;
        if (newDmg < 0)
            newDmg = 0;

        this.SetHealth("", type, newDmg);
    }

// ================================= FOLLOW TELEPORT =============================================

    void StartFollow(PlayerBase follower)
    {
        string followerId = follower.GetIdentity().GetId();
        if (CanBeFollowed() && m_FollowerIds.Find(followerId) == -1)
        {
            m_FollowerIds.Insert(followerId);
            m_IsFollowed = true;

            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StopFollow, m_FollowTime * 1000, false, followerId);
        }
    }

    void StopFollow(string followerId)
    {
        int followerIndex = m_FollowerIds.Find(followerId);
        if (followerIndex != -1)
            m_FollowerIds.RemoveItem(followerId);

        if (m_FollowerIds.Count() == 0)
            m_IsFollowed = false;
    }

    void SetFollowing(bool isFollowing)
    {
        m_IsFollowing = isFollowing;

        if (isFollowing)
        {
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetFollowing, m_FollowTime * 1000, false, false);
        }
        else
        {
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(SetFollowing);
        }
    }

    bool IsBeingFollowed()
    {
        return m_IsFollowed;
    }

    bool CanBeFollowed()
    {
        return !m_IsFollowing && m_FollowerIds.Count() < 3;
    }

    array<string> GetFollowerIds()
    {
        return m_FollowerIds;
    }

// ================================= SZ / FF =============================================

    override void Init()
    {
        super.Init();
        RegisterNetSyncVariableInt("netSync_safezoneStatus");
    }

    void SetSafezoneStatus(int status)
    {
        netSync_safezoneStatus = status;
        SetSynchDirty();
    }

    int GetSafezoneStatus()
    {
        return netSync_safezoneStatus;
    }

    override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
    {
        PlayerBase shooter = PlayerBase.Cast(source.GetHierarchyRootPlayer());
        PlayerBase target = this;

        if (target && (target.GetSafezoneStatus() == SZ_INSIDE || target.GetSafezoneStatus() == SZ_LEAVING || target.GetSafezoneStatus() == SPAWN_PROTECTION))
            return false;

        if (shooter && (shooter.GetSafezoneStatus() == SZ_INSIDE || shooter.GetSafezoneStatus() == SZ_LEAVING || shooter.GetSafezoneStatus() == SPAWN_PROTECTION))
            return false;

        if (shooter && shooter != target)
        {
            if (shooter.HasArmband() && target.HasArmband())
            {
                if (shooter.GetArmband() == target.GetArmband())
                    return false;
            }
        }

        return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
    }

// ================================= TEAM ARMBANDS =============================================

    void SetArmband(string teamID)
    {
        m_TeamID = teamID;
        if (teamID == NO_TEAM)
        {
            m_HasArmband = false;
        }
        else 
        {
            m_HasArmband = true;
        }
    }

    string GetArmband()
    {
        return m_TeamID;
    }

    bool HasArmband()
    {
        return m_HasArmband;
    }

    void RemoveArmband()
    {
        if (HasArmband())
        {
            PlayerHelper.DeleteTeamArmband(this);
            SetArmband(NO_TEAM);
        }
    }

// ================================= RESPAWN =============================================

    void InitArmband()
    {
        EntityAI armband = PlayerHelper.GetTeamArmband(this);
        if (armband)
        {
            SetArmband(armband.GetType());
        }
        else
        {
            SetArmband(NO_TEAM);
        }
    }

    override void OnConnect()
    {
        if (GetGame().IsServer())
        {
            InitArmband();
            m_EventState.SendEventStateToPlayer(this.GetIdentity());

            if (m_EventState.IsDayZ())
            {
                SetSafezoneStatus(NO_ZONE);
                m_EventTeams.SetPlayerAlive(this.GetIdentity(), true);
                RemoveArmband();
            }
            else if (m_EventState.IsKOTH())
            {
                HandleKOTHEvent();
            }
            else if (m_EventState.IsSnD())
            {
                SetSafezoneStatus(NO_ZONE);
                HandleSnDEvent();
            }
        }
        super.OnConnect();
    }

    void HandleKOTHEvent()
    {
        HandleVoting();
        HandleTeamSelection();
        KOTHManager.GetInstance().SendEventDataToPlayer(this.GetIdentity());
        m_EventTeams.SetPlayerAlive(this.GetIdentity(), true);
    }

    void HandleSnDEvent()
    {
        HandleVoting();
        HandleTeamSelection();
        SnDManager.GetInstance().SendEventDataToPlayer(this.GetIdentity());
        if (m_EventTeams.IsOnRedOrBlue(this.GetIdentity()))
        {
            string teamID = m_EventTeams.GetPlayerTeam(this.GetIdentity());
            if ((m_EventState.GetEventState() == SND_STATE_ACTIVE || m_EventState.GetEventState() == SND_STATE_ARMED))
            {
                m_EventTeams.SetPlayerAlive(this.GetIdentity(), false);
                NotificationSystem.SendNotificationToPlayerIdentityExtended(this.GetIdentity(), NOTIFY_LENGTH, NOTIFY_SND_TELEPORT);
            }
            else
            {
                m_EventTeams.SetPlayerAlive(this.GetIdentity(), true);
            }
        }
    }

    void HandleVoting()
    {
        if (m_EventTeams.VotingIsActive() && !m_EventTeams.HasVoted(this.GetIdentity().GetId()))
        {
            m_EventTeams.RequestSelection(this.GetIdentity(), REQUEST_VOTE);
        }
    }

    void HandleTeamSelection()
    {
        if (!m_EventTeams.IsPlayerOnTeam(this.GetIdentity()))
        {
            RemoveArmband();
            m_EventTeams.UpdatePlayerTeam(this.GetIdentity(), m_TeamID);
        }
        else if (m_EventTeams.IsPlayerUnassigned(this.GetIdentity()))
        {
            m_EventTeams.RequestSelection(this.GetIdentity(), REQUEST_TEAMSELECT);
        }
        else if (m_EventTeams.IsLeavingTeam(this.GetIdentity().GetId()))
        {
            RemoveArmband();
            m_EventTeams.UpdatePlayerTeam(this.GetIdentity(), m_TeamID);
        }
    }

    override void OnPlayerLoaded()
	{
		super.OnPlayerLoaded();

        RefreshWeapons();
        RefreshMagazines();
		m_PlayerLoaded = true;
	}

    void RefreshWeapons()
    {
        ref array<string> slotsToCheck = {"Shoulder", "Melee"};
        for (int i = 0; i < slotsToCheck.Count(); i++)
        {
            EntityAI attachment = FindAttachmentBySlotName(slotsToCheck[i]);
            Weapon_Base wpn_attachment = Weapon_Base.Cast(attachment);
            if (wpn_attachment)
                wpn_attachment.ValidateAndRepair();
        }

        ItemBase belt = ItemBase.Cast(FindAttachmentBySlotName("Hips"));
        if (belt)
        {
            ItemBase holster = ItemBase.Cast(belt.FindAttachmentBySlotName("Belt_Right"));
            if (holster)
            {
                Weapon_Base holsteredWeapon = Weapon_Base.Cast(holster.FindAttachmentBySlotName("Pistol"));
                if (holsteredWeapon)
                    holsteredWeapon.ValidateAndRepair();
            }
        }
    }

    void RefreshMagazines()
    {
        m_WeaponManager.SortMagazineAfterLoad();
    }

// ================================= OTHER =============================================


    bool HasItemInInventory(string classname)
    {
        array<EntityAI> items = new array<EntityAI>();
		GetInventory().EnumerateInventory(InventoryTraversalType.INORDER, items);
        foreach (EntityAI item : items)
        {
            if (item && item.GetType() == classname)
                return true;
        }

        return false;
    }

    void SpawnHeadlamp()
    {
        if (!GetGame().IsServer())
            return;

        if (IsAlive() && !IsPlayerDisconnected())
        {
            EntityAI headgear = this.FindAttachmentBySlotName("Eyewear");
            if (!headgear)
            {
                EntityAI headtorch = Headtorch_Grey.Cast(this.GetInventory().CreateInInventory("Headtorch_Grey"));
                if(headtorch)
                {
                    headtorch.GetInventory().CreateAttachment("Battery9V");
                }
            }
        }
    }

    void ResetPlayerHealth()
    {
        if (IsAlive() && !IsPlayerDisconnected())
        {
            SetHealthMax();
            SetHealthMax("", "blood");
            SetHealthMax("", "shock");
            m_BleedingManagerServer.RemoveAllSources();
        }
    }

    override void OnCommandDeathStart()
    {
        if (GetGame().IsServer())
            m_EventTeams.SetPlayerAlive(this.GetIdentity(), false);
        
        super.OnCommandDeathStart();
    }

    override override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (ammo == "FallDamage" && !IsAlive())
			IsFallDeath = true;
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	override void EEKilled(Object killer)
	{
		m_RewardManager.UpdatePlayerStat(this.GetIdentity(), "playerDeaths", 1);

        EntityAI weapon = null;
        PlayerBase target = null;
        DeathType deathType = DeathType.UNKNOWN;

        if (killer)
        {
            EntityAI killerEntity = EntityAI.Cast(killer);
            if (killerEntity)
                target = PlayerBase.Cast(killerEntity.GetHierarchyRootPlayer());

            if (killer == this)
            {
                if (GetHealth("", "Blood") < PlayerConstants.BLOOD_THRESHOLD_FATAL)
                    deathType = DeathType.BLEEDING;
                else if (IsFallDeath)
                    deathType = DeathType.FALL;
                else if (GetStatWater().Get() <= PlayerConstants.LOW_WATER_THRESHOLD || GetStatEnergy().Get() <= PlayerConstants.LOW_ENERGY_THRESHOLD)
                    deathType = DeathType.STARVING;
                else
                    deathType = DeathType.SUICIDE;
            }
            else if (killer.IsInherited(ZombieBase))
                deathType = DeathType.ZOMBIE;

            else if (killer.IsInherited(AnimalBase))
                deathType = DeathType.ANIMAL;

            else if (target && target != this)
            {
                if (killer != target)
                    weapon = EntityAI.Cast(killer);

                deathType = DeathType.PVP;
            }
        }

        GetKillfeed().OnPlayerKilled(deathType, this, target, weapon);

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

		if (playerKiller == this)
		    return; 

		if (playerKiller)
		{
			int currencyAmount = 1000;
            PlayerIdentity killerID = playerKiller.GetIdentity();
			CurrencyHelper helper = CurrencyHelper.GetInstance();
			helper.UpdateCurrency(killerID.GetPlainId(), currencyAmount, killerID);
			m_RewardManager.UpdatePlayerStat(killerID, "cashEarned", currencyAmount);
			m_RewardManager.UpdatePlayerStat(killerID, "playerKills", 1);
		}
	}

    override void BrokenLegForceProne(bool forceOverride = false)
    {
        if (!IsInWater() && !IsSwimming() && !IsClimbingLadder() && !IsInVehicle() && !IsClimbing())
        {
            EntityAI attachment;
            Class.CastTo(attachment, GetItemOnSlot("Splint_Right"));
            if (attachment && attachment.GetType() == "Splint_Applied")
                attachment.Delete();
            
            m_ShockHandler.SetShock(PlayerConstants.BROKEN_CROUCH_MODIFIER);
            m_ShockHandler.CheckValue(true);
            if (m_ShockHandler.GetCurrentShock() >= 25)
            {
                HumanCommandActionCallback cmd = GetCommand_Action();
                if (cmd) 
                    cmd.Cancel();

                StopHandEvent();
            }
        }
    }

	override void OnCommandSwimStart()
	{
		super.OnCommandSwimStart();
        
		if (GetInventory()) 
            GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
	}
	
	override void OnCommandSwimFinish()
	{
		if (GetInventory())
            GetInventory().LockInventory(LOCK_FROM_SCRIPT);

		super.OnCommandSwimFinish();
	}

	override void OnCommandVehicleStart()
	{
		super.OnCommandVehicleStart();

		if (GetInventory()) 
            GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
	}
	
	override void OnCommandVehicleFinish()
	{
		if (GetInventory())
            GetInventory().LockInventory(LOCK_FROM_SCRIPT);

		super.OnCommandVehicleFinish();
	}

	override bool CanReceiveItemIntoHands (EntityAI item_to_hands)
	{
		if (IsInVehicle())
		{
			EntityAI root = item_to_hands.GetHierarchyRoot();
			if (!root.IsTransport() && !root.IsPlayer())
				return false;
		}

		if (!CanPickupHeavyItem(item_to_hands))
			return false;

		return true;
	}

    override void OnRPC (PlayerIdentity sender, int rpc_type, ParamsReadContext ctx) 
    {
        switch (rpc_type) 
        {
            case RPC_CHAT_CLIENT_SEND_GLOBAL:
            {
                Param1<string> chatParams;
                if (!ctx.Read(chatParams))
                    return;

                string name = sender.GetName();
                string text = "[Global] " + name + " : " + chatParams.param1;
                ref array<Man> players = new array<Man>();
                GetGame().GetPlayers(players);
                for (int i = 0; i < players.Count(); ++i)
                {
                    if (players[i] && players[i].GetIdentity()) 
                    {
                        GetGame().RPCSingleParam(players[i], ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>(text), true, players[i].GetIdentity());
                    }
                }
                break;
            }
        }
        super.OnRPC (sender, rpc_type, ctx);

        if (GetGame().IsClient())
		{
			if (rpc_type == -74700005)
			{
				Param8<string, string, string, string, string, int, string, int> data;
				if (!ctx.Read(data))
					return;

				m_Killfeed.AddItem(data);
			}
		}
    }
}