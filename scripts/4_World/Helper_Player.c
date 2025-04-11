class PlayerHelper
{
    static PlayerBase GetPlayerByIdentity(PlayerIdentity sender)
    {
        int low = 0;
        int high = 0;
        GetGame().GetPlayerNetworkIDByIdentityID(sender.GetPlayerId(), low, high);
        return PlayerBase.Cast(GetGame().GetObjectByNetworkId(low, high));
    }

/////////////////////// ARMBANDS ///////////////////////

    static const ref array<string> Armbands = { TEAM_BLUE, TEAM_RED, TEAM_GREEN, TEAM_ORANGE, TEAM_YELLOW, TEAM_PINK };

    static bool IsEventTeam(string teamId)
    {
        return Armbands.Find(teamId) != -1;
    }

    static void EquipTeamArmband(PlayerIdentity sender, string teamID)
    {
        PlayerBase player = GetPlayerByIdentity(sender);
        if (!player || Armbands.Find(teamID) == -1)
            return;

        DeleteTeamArmband(player);
        EntityAI armband = EntityAI.Cast(player.GetInventory().CreateAttachment(teamID));
        player.SetArmband(teamID);
    }

    static EntityAI GetTeamArmband(PlayerBase player)
    {
        if (!player)
            return null;

        EntityAI armband = player.FindAttachmentBySlotName("Armband");
        if (armband)
        {
            if (Armbands.Find(armband.GetType()) > -1)
            {
                return armband;
            }
        }
        return null;
    }

    static void DeleteTeamArmband(PlayerBase player)
    {
        if (!player)
            return;

        EntityAI armband = player.FindAttachmentBySlotName("Armband");
        if (armband)
        {
            if (Armbands.Find(armband.GetType()) > -1)
            {
                GetGame().ObjectDelete(armband);
            }
        }
    }

/////////////////////// EVENTS ///////////////////////

    static void ResetToPos(PlayerIdentity identity, vector centerPos, float minRadius, float maxRadius, int posFlag, bool healPlayer = false)
    {
        PlayerBase player = GetPlayerByIdentity(identity);
        if (!player)
            return;

        vector resetPos = OffsetSpawnPos(centerPos, minRadius, maxRadius, posFlag);
        player.SetPosition(resetPos);

        if (healPlayer)
            player.ResetPlayerHealth();

        if (posFlag == POS_UNDERGROUND_ENOCH)
        {
            player.SpawnHeadlamp();
        }
    }

    static vector OffsetSpawnPos (vector pos, float minR, float maxR, int flag)
    {
        if (flag == POS_UNDERGROUND_ENOCH)
        {
            minR = 0.5;
            maxR = 2.5;
        }

        float randomRadius = Math.RandomFloatInclusive(minR, maxR);
        float randomAngle = Math.RandomFloatInclusive(0, Math.PI2);
        float offsetX = randomRadius * Math.Cos(randomAngle);
        float offsetZ = randomRadius * Math.Sin(randomAngle);

        vector resetPos = Vector(pos[0] + offsetX, pos[1], pos[2] + offsetZ);

        if (flag == POS_STANDARD)
            resetPos[1] = GetGame().SurfaceY(resetPos[0], resetPos[2]);

        return resetPos;
    }

    static bool RemoveItemFromInventory(PlayerIdentity identity, string className)
    {
        if (!identity)
            return false;

        PlayerBase player = GetPlayerByIdentity(identity);
        if (!player || !className)
            return false;

        array<EntityAI> itemsArray = new array<EntityAI>();
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);

        foreach (EntityAI item : itemsArray)
        {
            if (item && item.GetType() == className)
            {
                GetGame().ObjectDelete(item);
                return true;
            }
        }

        return false;
    }

    static void CheckForSnDBomb(PlayerBase player)
    {
        if (!player)
            return;

        array<EntityAI> itemsArray = new array<EntityAI>();
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);

        foreach (EntityAI item : itemsArray)
        {
            SnD_Bomb bomb = SnD_Bomb.Cast(item);
            if (bomb)
            {
                GetGame().ObjectDelete(bomb);

                SnDManager.GetInstance().BombDisconnected();
            }
        }
    }

/////////////////////// LOADOUT ///////////////////////

    static void EquipBelt(PlayerBase player, string hotkeyString)
    {
        EntityAI beltEnt = player.GetInventory().CreateInInventory("MilitaryBelt");
        EntityAI bandage1 = beltEnt.GetInventory().CreateAttachment("BandageDressing");
        SetPlayerHotkey(player, bandage1, "UABandageHotkey", hotkeyString);

        EntityAI knife = beltEnt.GetInventory().CreateAttachment("CombatKnife");
        SetPlayerHotkey(player, knife, "UAKnifeHotkey", hotkeyString);
    }

	static void AddPlayerStats(PlayerBase player)
	{
		player.GetStatWater().Add(1900);
		player.GetStatEnergy().Add(1900);
	}

    static void CreateMultipleItems(PlayerBase player, string itemName, int count)
	{
		for (int i = 0; i < count; i++)
		{
            player.GetInventory().CreateInInventory(itemName);
        }
	}

    static string ParseItemTypeFromLoadout(string loadoutString, int targetCategoryIndex)
    {
        TStringArray loadoutEntries = new TStringArray;
        loadoutString.Split(" ", loadoutEntries);
        foreach (string entry : loadoutEntries)
        {
            TStringArray parts = new TStringArray;
            entry.Split(";", parts);
            if (parts.Count() == 2)
            {
                int categoryIndex = parts[0].ToInt();
                int itemIndex = parts[1].ToInt();
                if (categoryIndex == targetCategoryIndex)
                {
                    ref array<ref SpawnConfig> categoryConfigs;
                    switch (categoryIndex)
                    {
                        case 0:
                            categoryConfigs = SpawnMenuItems.Character();
                            break;
                        case 1:
                            categoryConfigs = SpawnMenuItems.Sidearm();
                            break;
                        case 2:
                            categoryConfigs = SpawnMenuItems.Torso();
                            break;
                        case 3:
                            categoryConfigs = SpawnMenuItems.Legs();
                            break;
                        case 4:
                            categoryConfigs = SpawnMenuItems.Feet();
                            break;
                        case 5:
                            categoryConfigs = SpawnMenuItems.Backpack();
                            break;
                        case 6:
                            categoryConfigs = SpawnMenuItems.Hands();
                            break;
                        case 7:
                            categoryConfigs = SpawnMenuItems.Head();
                            break;
                        default:
                            return "Unknown";
                    }

                    if (categoryConfigs && itemIndex >= 0 && itemIndex < categoryConfigs.Count())
                    {
                        return categoryConfigs[itemIndex].classname;
                    }
                }
            }
        }

        return "Unknown";
    }

    static void SetPlayerHotkey(PlayerBase player, EntityAI item, string actionName, string hotkeyString)
    {
        PlayerIdentity identity = player.GetIdentity();
        if (!identity)
        {
            return;
        }

        TStringArray hotkeyArray = new TStringArray;
        hotkeyString.Split(",", hotkeyArray);

        int hotkeyIndex = GetHotkeyIndexFromActionName(actionName);
        if (hotkeyIndex >= 0 && hotkeyIndex < hotkeyArray.Count())
        {
            int quickbarIndex = hotkeyArray[hotkeyIndex].ToInt() - 1;
            if (quickbarIndex >= 0 && quickbarIndex <= 9)
            {
                player.SetQuickBarEntityShortcut(item, quickbarIndex, true);
            }
        }
    }

    static int GetHotkeyIndexFromActionName(string actionName)
    {
        switch (actionName)
        {
            case "UASidearmHotkey":
                return 0;
            case "UABandageHotkey":
                return 1;
            case "UAKnifeHotkey":
                return 2;
            default:
                return -1;
        }
        return -1;
    }

    static string GenerateRandomCharacter()
    {
        string loadout = "";
    
        ref array<ref SpawnConfig> characters = SpawnMenuItems.Character();
        int randomCharacterIndex = Math.RandomInt(0, characters.Count());
        loadout += "0;" + randomCharacterIndex.ToString() + " ";

        loadout += "1;0 ";

        ref array<ref SpawnConfig> torsoItems = SpawnMenuItems.Torso();
        int randomTorsoIndex = GetRandomIndexForLevelRequirement(torsoItems, 0);
        loadout += "2;" + randomTorsoIndex.ToString() + " ";

        ref array<ref SpawnConfig> legItems = SpawnMenuItems.Legs();
        int randomLegsIndex = GetRandomIndexForLevelRequirement(legItems, 0);
        loadout += "3;" + randomLegsIndex.ToString() + " ";

        ref array<ref SpawnConfig> feetItems = SpawnMenuItems.Feet();
        int randomFeetIndex = GetRandomIndexForLevelRequirement(feetItems, 0);
        loadout += "4;" + randomFeetIndex.ToString() + " ";

        loadout += "5;-1 6;-1 7;-1";

        return loadout.Trim();
    }

    static int GetRandomIndexForLevelRequirement(ref array<ref SpawnConfig> items, int requiredLevel)
    {
        ref array<int> validIndices = new array<int>();
        for (int i = 0; i < items.Count(); i++)
        {
            if (items[i].levelRequirement == requiredLevel)
                validIndices.Insert(i);
        }

        if (validIndices.Count() > 0)
            return validIndices.Get(Math.RandomInt(0, validIndices.Count()));

        return -1;
    }

    static string GetSidearmRelatedItem(string classname)
    {
        ref array<ref SpawnConfig> sidearms = SpawnMenuItems.Sidearm();

        foreach (SpawnConfig config : sidearms)
        {
            if (config.classname == classname)
                return config.relatedItem;
        }
        return "";
    }
}