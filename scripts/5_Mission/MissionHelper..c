class MissionHelper
{
    void ProcessPurchaseOrder(PlayerIdentity identity, TStringArray orderItems)
    {
        PlayerBase player = PlayerBase.Cast(identity.GetPlayer());
        if (!player)
            return;

        foreach (string order : orderItems)
        {
            TStringArray itemData = new TStringArray;
            order.Split(";", itemData);
            if (itemData.Count() == 2)
            {
                int itemIndex = itemData.Get(0).ToInt();
                int qty = itemData.Get(1).ToInt();
                ItemConfig config = itemConfigs[itemIndex];
                for (int i = 0; i < qty; i++)
                {
                    if (config.category == "Gear" && config.hasrelated == 3)
                    {
                        EquipVestWithPouches(player, config);
                    }

                    else if (config.category == "Weapons" || config.category == "LongWeapons")
                    {
                        EntityAI weaponEntity = SpawnWeaponWithMagazine(player, config);
                        if (weaponEntity)
                        {
                            EquipWeaponAttachments(player, weaponEntity, config.classname);
                        }
                    }

                    else if (config.category == "Gear" && config.hasrelated == 4)
                    {
                        EquipHelmetWithVisor(player, config);
                    }

                    else if ((config.category == "Attachments" || config.category == "Other") && config.hasrelated == 5)
                    {
                        EquipOpticWithBattery(player, config);
                    }

                    else
                    {
                        SpawnItemNearPlayer(player, config.classname);
                    }
                }
            }
        }
    }

    EntityAI SpawnItemNearPlayer(PlayerBase player, string itemClassname)
    {
        EntityAI itemEntity = player.GetInventory().CreateInInventory(itemClassname);
        if (itemEntity)
        {
            player.RefreshMagazines();
        }
        else
        {
            vector playerPosition = player.GetPosition();
            vector spawnPosition = playerPosition + "0 0.1 0";
            itemEntity = EntityAI.Cast(GetGame().CreateObject(itemClassname, spawnPosition, false, true));
        }

        return itemEntity;
    }

    void EquipVestWithPouches(PlayerBase player, ItemConfig vestConfig)
    {
        EntityAI vestEntity = SpawnItemNearPlayer(player, vestConfig.classname);
        if (!vestEntity)
            return;

        Clothing vest = Clothing.Cast(vestEntity);
        if (vest) 
        {
            string pouchClassname = vestConfig.related;
            if (pouchClassname != "")
            {
                CreateItemInEntity(vest, pouchClassname);
            }
        }
    }

    EntityAI SpawnWeaponWithMagazine(PlayerBase player, ItemConfig weaponConfig)
    {
        EntityAI weaponEntity = SpawnItemNearPlayer(player, weaponConfig.classname);
        if (!weaponEntity) 
        {
            return null;
        }

        Weapon_Base weaponInstance = Weapon_Base.Cast(weaponEntity);
        if (!weaponInstance) 
        {
            return null;
        }

        if (weaponConfig.hasrelated == 1)
        {
            string magClassname = weaponConfig.related;
            if (magClassname != "")
            {
                weaponInstance.SpawnAttachedMagazine(magClassname);
            }
        }

        else if (weaponConfig.hasrelated == 2)
        {
            string ammoClassname = weaponConfig.related;
            if (ammoClassname != "")
            {
                if (weaponEntity.GetType() == "Izh43Shotgun" || weaponEntity.GetType() == "B95")
                {
                    weaponInstance.FillChamber(ammoClassname);
                }
                else
                {
                    weaponInstance.FillInnerMagazine(ammoClassname);
                }
            }
        }

        if (weaponInstance)
        {
            int quickBarSize = player.GetQuickBarSize();
            for (int i = 0; i < quickBarSize; i++)
            {
                if (!player.GetQuickBarEntity(i))
                {
                    player.SetQuickBarEntityShortcut(weaponInstance, i, true);
                    break;
                }
            }
        }
        return weaponEntity;
    }

    void EquipHelmetWithVisor(PlayerBase player, ItemConfig helmetConfig)
    {
        EntityAI helmetEntity = SpawnItemNearPlayer(player, helmetConfig.classname);
        if (!helmetEntity) 
        {
            return;
        }

        Clothing helmet = Clothing.Cast(helmetEntity);
        if (helmet) 
        {
            string visorClassname = helmetConfig.related;
            if (visorClassname != "")
            {
                CreateItemInEntity(helmet, visorClassname);
            }
        }
    }

    void EquipWeaponAttachments(PlayerBase player, EntityAI weaponEntity, string weaponClassname)
    {
        Weapon_Base weaponInstance = Weapon_Base.Cast(weaponEntity);
        if (!weaponInstance)
        {
            return;
        }

        switch (weaponClassname)
        {
            case "MP5K":
                weaponInstance.GetInventory().CreateAttachment("MP5_PlasticHndgrd");
                weaponInstance.GetInventory().CreateAttachment("MP5k_StockBttstck");
                weaponInstance.GetInventory().CreateAttachment("MP5_Compensator");
                break;

            case "PP19":
                weaponInstance.GetInventory().CreateAttachment("PP19_Bttstck");
                break;

            case "AKS74U":
                weaponInstance.GetInventory().CreateAttachment("AKS74U_Bttstck");
                break;

            case "AK74":
                weaponInstance.GetInventory().CreateAttachment("AK74_Hndgrd");
                weaponInstance.GetInventory().CreateAttachment("AK_PlasticBttstck");
                break;

            case "AK101":
                weaponInstance.GetInventory().CreateAttachment("AK_PlasticHndgrd");
                weaponInstance.GetInventory().CreateAttachment("AK_PlasticBttstck");
                break;

            case "AKM":
                weaponInstance.GetInventory().CreateAttachment("AK_RailHndgrd");
                weaponInstance.GetInventory().CreateAttachment("AK_PlasticBttstck");
                break;

            case "M4A1":
                weaponInstance.GetInventory().CreateAttachment("M4_OEBttstck");
                weaponInstance.GetInventory().CreateAttachment("M4_PlasticHndgrd");
                break;

            case "FAL":
                weaponInstance.GetInventory().CreateAttachment("Fal_OeBttstck");
                break;

            default:
                break;
        }
    }

    void EquipOpticWithBattery(PlayerBase player, ItemConfig opticConfig)
    {
        EntityAI opticEntity = player.GetInventory().CreateInInventory(opticConfig.classname);
        if (!opticEntity) 
        {
            return;
        }

        ItemBase optic = ItemBase.Cast(opticEntity);
        if (optic) 
        {
            string batteryClassname = opticConfig.related;
            if (batteryClassname != "")
            {
                CreateItemInEntity(optic, batteryClassname);
            }
        }
    }

	EntityAI CreateItemInEntity(EntityAI entity, string itemName)
	{ 
        return entity.GetInventory().CreateInInventory(itemName); 
    }
}