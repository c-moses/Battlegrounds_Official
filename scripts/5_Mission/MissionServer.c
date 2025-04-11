modded class MissionServer
{
    protected ref MissionHelper m_MissionHelper;
    protected float m_EventTick = 0.0;
    protected ref BattlegroundsConfig m_LeaderboardConfig;
    protected ref BattlegroundsRewardManager m_RewardManager;
    protected ref CurrencyConfigLoader m_CurrencyConfig;
    protected ref CurrencyHelper m_CurrencyHelper;
    protected ref EventState m_EventState;
    protected ref EventTeams m_EventTeams;

    protected ref map<string, string> m_PlayerLoadouts = new map<string, string>;
    protected ref map<string, string> m_PlayerHotkeys = new map<string, string>;

    protected string m_Moses = "76561198106483083";

// ================================= BARTER =============================================

    void BarterTransaction(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Server || sender == null)
            return;

        Param1<string> data;
        if (!ctx.Read(data))
            return;

        string barterItem = data.param1;
        string senderSteamID = sender.GetPlainId();
        if (m_CurrencyHelper)
        {
            BarterInfo matchedItem = BarterConfig.GetBarterItemByClassname(barterItem);
            if (matchedItem && PlayerHelper.RemoveItemFromInventory(sender, barterItem))
            {
                int worth = matchedItem.worth;
                m_CurrencyHelper.UpdateCurrency(senderSteamID, worth, sender);
                m_RewardManager.UpdatePlayerStat(sender, "cashEarned", worth);

                string notifySuccessMessage = "$" + worth.ToString() + NOTIFY_BARTER_SUCCESS_SENT;
                NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, NOTIFY_LENGTH, NOTIFY_BARTER_SUCCESS, notifySuccessMessage);
            }
        }
    }

// ================================= GAME MASTER =============================================

    void GameMaster(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Server)
            return;

        Param6<string, float, int, float, float, int> param;
        if (!ctx.Read(param))
            return;

        if (param.param1 != "")
        {
            string layerMode = param.param1;
            float roundTime = param.param2;
            int scoreLimit = param.param3;
            float bombTime = param.param4;
            float preRoundTime = param.param5;
            int roundSwitch = param.param6;

            LayerConfigLoader.UpdateLayerConfig("$profile:Battlegrounds/Layer.json", layerMode, roundTime, scoreLimit, bombTime, preRoundTime, roundSwitch);
        }

        m_EventState.StartNewEvent(false);
    }

// ================================= INIT =============================================

    void MissionServer()
    {
        ServerRPC.InitServerRPCs();
        m_LeaderboardConfig = BattlegroundsConfigLoader.GetConfig();
        GetDayZGame().SetBattlegroundsConfig(m_LeaderboardConfig);
        GetDayZGame().SetBGPlayerStatsMap(m_LeaderboardConfig);
    }

    override void OnInit()
    {
        super.OnInit();

        m_MissionHelper = new MissionHelper();
        InitEventState();
        InitRewardManagers();

        GetRPCManager().AddRPC("Battlegrounds", "LeaderboardSync", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("LoadoutSelect", "RequestLoadout", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("BuyMenu", "BuyMenuTransaction", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("TransferMenu", "TransferFunds", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("TransferMenu", "BarterTransaction", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("Battlegrounds", "RequestTeam", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("Battlegrounds", "GameMaster", this, SingleplayerExecutionType.Server);
        

        if (m_LeaderboardConfig && m_LeaderboardConfig.Players)
            BGLeaderboardManager.GetInstance(m_LeaderboardConfig.Players);

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CleanupBattlegrounds, 8000, false);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InitBattlegrounds, 20000, false);
    }

    void CleanupBattlegrounds()
    {
        m_EventState.CleanupBattlegrounds();
    }

    void InitBattlegrounds()
    {
        m_EventState.InitBattlegrounds();
        m_EventState.StartNewEvent(true);
    }

    void InitRewardManagers()
    {
        m_CurrencyHelper = CurrencyHelper.GetInstance();
        m_RewardManager = BattlegroundsRewardManager.GetInstance();
    }

    void InitEventState()
    {
        string world = GetGame().GetWorldName();
        m_EventState = EventState.GetInstance();
        m_EventState.Init(world);

        m_EventTeams = EventTeams.GetInstance();
        m_EventTeams.Init();
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        if (!GetGame() || !m_EventState)
            return;

        m_EventTick += timeslice;
        if (m_EventTick >= EVENT_TICK)
        {
            m_EventState.EventTick();
            m_EventTick = 0.0;
        }
    }

// ================================= CONNECT/DISCONNECT =============================================

    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);

        if (player && identity)
        {
            m_CurrencyHelper.InitPlayerCurrency(player, identity);
            RetrievePlayerStats(identity);
        }

        if (identity.GetPlainId() == m_Moses)
        {
            GetRPCManager().SendRPC("Battlegrounds", "ReceiveGameMaster", new Param1<bool>(true), true, identity);
        }
    }

    override void PlayerDisconnected(PlayerBase player, PlayerIdentity identity, string uid)
    {
        if (!player)
            return;

        PlayerHelper.CheckForSnDBomb(player);
        m_EventTeams.RemovePlayer(uid);
        m_CurrencyHelper.RemovePlayerCurrency(uid);

        super.PlayerDisconnected(player, identity, uid);
    }

// ================================= PLAYER STATS =============================================

    void RetrievePlayerStats(PlayerIdentity identity)
    {
        if (identity)
        {
            string playerId = identity.GetId();
            string playerName = identity.GetName();
            DayZGame game = DayZGame.Cast(GetGame());
            if (game)
            {
                BGPlayerStats playerStats = game.GetBattlegroundsConfig().FindPlayerStats(playerId, playerName);
                if (playerStats)
                {
                    LevelManager levelManager = new LevelManager();
                    int playerLevel = levelManager.GetPlayerLevel(playerStats.cashEarned);
                    Param5<int, int, int, int, int> param = new Param5<int, int, int, int, int>
                    (playerStats.playerKills, playerStats.zombieKills, playerStats.deaths, playerStats.cashEarned, playerLevel);
                    GetRPCManager().SendRPC("Battlegrounds", "ReceiveStats", param, true, identity);
                }
            }
        }
    }

// ================================= TEAM SELECT =============================================

    void RequestTeam(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Server)
            return;

        Param2<string, bool> param;
        if (!ctx.Read(param))
            return;

        if (!sender)
            return;
            
        string teamID = param.param1;
        bool leavingTeam = param.param2;

        if (leavingTeam)
        {
            m_EventTeams.RequestLeaveTeam(sender.GetId());
            return;
        }

        m_EventTeams.UpdatePlayerTeam(sender, teamID);
        PlayerHelper.EquipTeamArmband(sender, teamID);
        if (m_EventState.IsSnD())
        {
            if (m_EventState.GetEventState() == SND_STATE_ACTIVE_BUY)
            {
                if (teamID == TEAM_BLUE)
                    PlayerHelper.ResetToPos(sender, m_EventState.BlueSpawn(), 1.0, 5.0, m_EventState.GetResetPosFlag());

                else if (teamID == TEAM_RED)
                    PlayerHelper.ResetToPos(sender, m_EventState.RedSpawn(), 1.0, 5.0, m_EventState.GetResetPosFlag());
            }

            if (m_EventState.GetEventState() == SND_STATE_ACTIVE || m_EventState.GetEventState() == SND_STATE_ARMED)
            {
                m_EventTeams.SetPlayerAlive(sender, false);
                NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, NOTIFY_LENGTH, NOTIFY_SND_TELEPORT);
            }
        }
    }

// ================================= TRANSFER =============================================

    void TransferFunds(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Server || sender == null)
            return;

        Param2<string, int> data;
        if (!ctx.Read(data))
            return;

        string recipientSteamID = data.param1;
        int transferAmount = data.param2;
        string senderSteamID = sender.GetPlainId();
        if (!CurrencyConfigLoader.DoesPlayerCurrencyExist(recipientSteamID))
        {
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, NOTIFY_LENGTH, NOTIFY_TRANSFER_FAILED, NOTIFY_TRANSFER_FAILED_NOPLAYER);
            return;
        }

        int senderCurrency = m_CurrencyHelper.GetPlayerCurrency(senderSteamID);
        if (senderCurrency >= transferAmount)
        {
            m_CurrencyHelper.UpdateCurrency(senderSteamID, -transferAmount, sender);
            m_CurrencyHelper.UpdateCurrency(recipientSteamID, transferAmount);

            string notifySuccessMessage = "$" + transferAmount.ToString() + NOTIFY_TRANSFER_SUCCESS_SENT;
            NotificationSystem.SendNotificationToPlayerIdentityExtended(sender, NOTIFY_LENGTH, NOTIFY_TRANSFER_SUCCESS, notifySuccessMessage);
        }
    }

// ================================= BUY MENU =============================================

    void BuyMenuTransaction(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Server || sender == null)
            return;

        Param1<string> param;
        if (ctx.Read(param)) 
        {
            string playerName = sender.GetName();
            string steamID = sender.GetPlainId();
            string purchaseOrder = param.param1;

            int currentCurrency = m_CurrencyHelper.GetPlayerCurrency(steamID);

            int totalCost = 0;
            TStringArray itemOrders = new TStringArray;
            purchaseOrder.Split(" ", itemOrders);

            foreach (string order : itemOrders)
            {
                TStringArray itemData = new TStringArray;
                order.Split(";", itemData);

                if (itemData.Count() == 2)
                {
                    int itemIndex = itemData.Get(0).ToInt();
                    int qty = itemData.Get(1).ToInt();
                    if (itemIndex >= 0 && itemIndex < itemConfigs.Count())
                    {
                        ItemConfig item = itemConfigs[itemIndex];
                        int itemCost = item.price * qty;
                        totalCost += itemCost;
                    }
                }
            }

            if (currentCurrency >= totalCost)
            {
                m_CurrencyHelper.UpdateCurrency(steamID, -totalCost, sender);
                m_MissionHelper.ProcessPurchaseOrder(sender, itemOrders);
            }
        }
    }

// ================================= LOADOUT =============================================

    void RequestLoadout(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Server)
            return;

        Param2<string, string> data;
        if (!ctx.Read(data))
            return;

        if (sender == null)
            return;

        string loadoutString = data.param1;
        if (m_PlayerLoadouts.Contains(sender.GetId()))
        {
            m_PlayerLoadouts.Remove(sender.GetId());
        }

        string hotkeyString = data.param2;
        if (m_PlayerHotkeys.Contains(sender.GetId()))
        {
            m_PlayerHotkeys.Remove(sender.GetId());
        }

        m_PlayerLoadouts.Set(sender.GetId(), loadoutString);
        m_PlayerHotkeys.Set(sender.GetId(), hotkeyString);
    }

    void EquipLoadout(PlayerBase player, string loadoutString, string sidearmType)
    {
        PlayerHelper.AddPlayerStats(player);

        PlayerIdentity identity = player.GetIdentity();
        string playerID = identity.GetId();
        string hotkeyString;

        if (m_PlayerHotkeys.Contains(playerID))
        {
            hotkeyString = m_PlayerHotkeys.Get(playerID);
        }
        else
        {
            hotkeyString = "3,5,4";
        }

        PlayerHelper.EquipBelt(player, hotkeyString);

        for (int categoryIndex = 2; categoryIndex <= 7; categoryIndex++)
        {
            string clothesClassname = PlayerHelper.ParseItemTypeFromLoadout(loadoutString, categoryIndex);
            if (clothesClassname != "Unknown" && clothesClassname != "")
            {
                EntityAI itemInstance = player.GetInventory().CreateInInventory(clothesClassname);
            }
        }

        if (sidearmType != "Unknown")
        {
            string relatedItem = PlayerHelper.GetSidearmRelatedItem(sidearmType);
            Weapon_Base weapon;
            if (sidearmType == "CZ61")
            {
                weapon = Weapon_Base.Cast(player.GetHumanInventory().CreateInHands(sidearmType));
            }
            else
            {
                EntityAI holster = player.GetInventory().CreateInInventory("PlateCarrierHolster_Black");
                weapon = Weapon_Base.Cast(player.GetHumanInventory().CreateInHands(sidearmType));
                InventoryLocation oldLoc = new InventoryLocation;
                InventoryLocation newLoc = new InventoryLocation;
                weapon.EEItemLocationChanged(oldLoc, newLoc);
            }

            if (weapon)
            {
                PlayerHelper.SetPlayerHotkey(player, weapon, "UASidearmHotkey", hotkeyString);

                if (relatedItem != "")
                {
                    if (relatedItem.Contains("Mag_"))
                    {
                        weapon.SpawnAttachedMagazine(relatedItem);
                    }
                    else if (relatedItem.Contains("Ammo_"))
                    {
                        weapon.SpawnAmmo(relatedItem);
                    }

                    PlayerHelper.CreateMultipleItems(player, relatedItem, 3);
                }

                string weaponClassname = weapon.GetType();
                if (weaponClassname == "Deagle" || weaponClassname == "Deagle_Gold" || weaponClassname == "LongHorn")
                {
                    weapon.GetInventory().CreateAttachment("PistolOptic");
                }
            }
        }
    }

// ================================= RESPAWN =============================================

    override PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx)
    {
        if (ProcessLoginData(ctx))
        {
            string sidearmType;
            string randomSidearmType;
            string loadoutString;
            string randomLoadout;
            string customCharacterType;
            string characterType = GetGame().CreateRandomPlayer();
            if (m_EventState.IsDayZ())
            {
                if (CreateCharacter(identity, pos, ctx, characterType))
                {
                    GetGame().GetMenuDefaultCharacterData().GenerateRandomEquip();
                    EquipCharacter(GetGame().GetMenuDefaultCharacterData());
                }
            }
            else if (m_EventState.IsKOTH())
            {
                vector spawnPos = PlayerHelper.OffsetSpawnPos(m_EventState.MainSpawn(), 3.0, 12.0, m_EventState.GetResetPosFlag());
                if (m_RespawnMode == GameConstants.RESPAWN_MODE_CUSTOM && m_PlayerLoadouts.Contains(identity.GetId()))
                {
                    loadoutString = m_PlayerLoadouts.Get(identity.GetId());
                    customCharacterType = PlayerHelper.ParseItemTypeFromLoadout(loadoutString, 0);
                    sidearmType = PlayerHelper.ParseItemTypeFromLoadout(loadoutString, 1);
                    m_player = CreateCharacter(identity, spawnPos, ctx, customCharacterType);
                    EquipLoadout(m_player, loadoutString, sidearmType);
                }
                else
                {
                    randomLoadout = PlayerHelper.GenerateRandomCharacter();
                    randomSidearmType = PlayerHelper.ParseItemTypeFromLoadout(randomLoadout, 1);
                    m_player = CreateCharacter(identity, spawnPos, ctx, characterType);
                    EquipLoadout(m_player, randomLoadout, randomSidearmType);
                }

                if (!m_EventTeams.IsPlayerUnassigned(identity))
                {
                    string armbandID = m_EventTeams.GetPlayerTeam(identity);
                    PlayerHelper.EquipTeamArmband(identity, armbandID);
                }
            }
            else if (m_EventState.IsSnD())
            {
                if (m_RespawnMode == GameConstants.RESPAWN_MODE_CUSTOM && m_PlayerLoadouts.Contains(identity.GetId()))
                {
                    loadoutString = m_PlayerLoadouts.Get(identity.GetId());
                    customCharacterType = PlayerHelper.ParseItemTypeFromLoadout(loadoutString, 0);
                    sidearmType = PlayerHelper.ParseItemTypeFromLoadout(loadoutString, 1);
                    m_player = CreateCharacter(identity, pos, ctx, customCharacterType);
                    EquipLoadout(m_player, loadoutString, sidearmType);
                }
                else
                {
                    randomLoadout = PlayerHelper.GenerateRandomCharacter();
                    randomSidearmType = PlayerHelper.ParseItemTypeFromLoadout(randomLoadout, 1);
                    m_player = CreateCharacter(identity, pos, ctx, characterType);
                    EquipLoadout(m_player, randomLoadout, randomSidearmType);
                }

                if (m_EventTeams.IsOnRedOrBlue(identity))
                {
                    string teamID = m_EventTeams.GetPlayerTeam(identity);
                    if (m_EventState.GetEventState() == SND_STATE_ACTIVE_BUY)
                    {
                        int resetFlag = m_EventState.GetResetPosFlag();

                        if(teamID == TEAM_RED)
                            pos = PlayerHelper.OffsetSpawnPos(m_EventState.RedSpawn(), 1.0, 6.0, resetFlag);
                        else pos = PlayerHelper.OffsetSpawnPos(m_EventState.BlueSpawn(), 1.0, 6.0, resetFlag);

                        if (resetFlag == POS_UNDERGROUND_ENOCH)
                        {
                            m_player.SpawnHeadlamp();
                        }
                    }

                    PlayerHelper.EquipTeamArmband(identity, teamID);
                }
            }
        }

        return m_player;
    }

// ================================= LEADERBOARD =============================================

    void LeaderboardSync(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type == CallType.Server)
        {
            DayZGame game = DayZGame.Cast(GetGame());
            BattlegroundsConfig config = game.GetBattlegroundsConfig();

            if (config && config.Players.Count() > 0)
            {
                BGLeaderboardManager manager = BGLeaderboardManager.GetInstance();
                
                if (manager)
                {
                    manager.SetPlayers(config.Players);
                    manager.QuickSortByCashEarned(0, config.Players.Count() - 1);

                    array<BGPlayerStats> sortedPlayers = manager.GetSortedList("desc");
                    int countToSend = Math.Min(50, sortedPlayers.Count());

                    array<BGPlayerStats> sortedPlayersSubset = SliceArray(sortedPlayers, 0, countToSend);
                    Param1<array<BGPlayerStats>> data = new Param1<array<BGPlayerStats>>(sortedPlayersSubset);

                    GetRPCManager().SendRPC("Battlegrounds", "ReceiveLeaderboard", data, true, sender);
                }
            }
        }
    }

    array<BGPlayerStats> SliceArray(array<BGPlayerStats> inputArray, int startIndex, int endIndex)
    {
        array<BGPlayerStats> result = new array<BGPlayerStats>();
        for (int i = startIndex; i < endIndex && i < inputArray.Count(); i++)
        { result.Insert(inputArray[i]); }
        return result;
    }
}