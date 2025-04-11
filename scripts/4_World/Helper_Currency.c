class CurrencyHelper
{
	static ref CurrencyHelper m_Instance;
	static bool m_Inited = false;
	protected ref map<string, int> m_PlayerCurrency;
	protected ref map<string, string> m_UserIDToSteamID;
	private float m_TimeAccumulator = 0;
    private const float UPDATE_INTERVAL = 0.2;

	/*void AddCurrencyStackToPlayer(PlayerBase player, int amount)
	{
		PlayerIdentity identity = player.GetIdentity();
		string steamID = identity.GetPlainId();
		UpdateCurrency(steamID, amount, identity);
	}*/

	void UpdateCurrency(string steamID, int amountChange, PlayerIdentity identity = null)
	{
		if (m_PlayerCurrency.Contains(steamID))
		{
			int newCurrency = m_PlayerCurrency.Get(steamID) + amountChange;
			m_PlayerCurrency.Set(steamID, newCurrency);
		}

		PlayerCurrency currency;
		if (!CurrencyConfigLoader.DoesPlayerCurrencyExist(steamID))
		{
			string playerName = "Unknown";
			if (identity)
				playerName = identity.GetName();

			currency = CurrencyConfigLoader.CreateDefaultPlayerCurrency(steamID, playerName);
		}
		else
		{
			currency = CurrencyConfigLoader.GetPlayerCurrency(steamID);
			if (identity && currency.PlayerName == "")
			{
				string newName = identity.GetName();
				if (newName != "")
					currency.PlayerName = newName;
			}
		}

		currency.Currency += amountChange;
		CurrencyConfigLoader.SavePlayerCurrency(currency);

		if (identity == null)
			identity = GetPlayerIdentityBySteamID(steamID);

		if (identity)
			SendUpdatedCurrencyToPlayer(currency.Currency, identity);
	}

	void SendUpdatedCurrencyToPlayer(int newCurrencyAmount, PlayerIdentity identity)
	{
		Param1<int> param = new Param1<int>(newCurrencyAmount);
		GetRPCManager().SendRPC("Battlegrounds", "ReceiveCurrency", param, true, identity);
	}

	// UTILITY

	int GetPlayerCurrency(string steamID)
	{
		if (!m_PlayerCurrency.Contains(steamID)) 
		{
			// will call on disconnect
			return -1;
		}
		return m_PlayerCurrency.Get(steamID);
	}

	void InitPlayerCurrency(PlayerBase player, PlayerIdentity identity)
	{
		if (!player)
        	return;

		string userID = identity.GetId();
		string steamID = identity.GetPlainId();
		string playerName = identity.GetName();
		PlayerCurrency currency = CurrencyConfigLoader.GetOrCreatePlayerCurrency(steamID, playerName);
		
		if (currency) 
		{
			if (!m_PlayerCurrency.Contains(steamID))
				m_PlayerCurrency.Set(steamID, currency.Currency);
				
			if (!m_UserIDToSteamID.Contains(userID))
                m_UserIDToSteamID.Set(userID, steamID);

			SendUpdatedCurrencyToPlayer(currency.Currency, identity);

			//Print("[CurrencyHelper] InitPlayerCurrency: Loaded " + steamID + "'s currency");
		}
	}

	void RemovePlayerCurrency(string userID)
	{
		if (userID == "")
			return;

		string steamID;
		if (!m_UserIDToSteamID.Find(userID, steamID) || steamID == "")
			return;

		int currentCurrency = GetPlayerCurrency(steamID);
		if (currentCurrency != -1)
		{
			PlayerCurrency currency = CurrencyConfigLoader.GetOrCreatePlayerCurrency(steamID, "");
			currency.Currency = currentCurrency;
			CurrencyConfigLoader.SavePlayerCurrency(currency);

			//Print("[CurrencyHelper] RemovePlayerCurrency: Saved " + steamID + "'s currency");
		}

		if (m_PlayerCurrency.Contains(steamID))
		{
			m_PlayerCurrency.Remove(steamID);
		}
	}

	PlayerIdentity GetPlayerIdentityBySteamID(string steamID)
	{
		array<Man> players = new array<Man>();
		GetGame().GetPlayers(players);
		
		foreach (Man player : players)
		{
			PlayerIdentity identity = player.GetIdentity();
			if (identity && identity.GetPlainId() == steamID)
				return identity;
		}
		
		return null;
	}

	/*void OnUpdate(float timeslice)
	{
		if (!GetGame() || !m_Inited)
			return;

		m_TimeAccumulator += timeslice;
		if (m_TimeAccumulator >= UPDATE_INTERVAL)
		{
			m_TimeAccumulator = 0;
			// process queue (worry about this later)
		}
	}*/

	// INSTANCE

	void CurrencyHelper()
    { 
		m_PlayerCurrency = new map<string, int>(); 
		m_UserIDToSteamID = new map<string, string>(); 
	}

	static CurrencyHelper GetInstance()
    {
        if (!m_Instance)
        { 
			m_Instance = new CurrencyHelper();
			m_Inited = true;
		}
        return m_Instance;
    }
} 

	// ////////////////////////////////////////////////////////////////

	/*	depracated method. note: takes 3rd argument (classname)
	
	const int MAX_CURRENCY_STACK = 100000;

	void AddCurrencyStackToPlayer(PlayerBase player, string classname, int amount)
	{
		int remainingAmount = amount;

		//Print("[BattlegroundsRewards] Looping until all currency is added to player");
		while (remainingAmount > 0)
		{
			ItemBase existingCurrency = FindItem(player, classname);

			if (existingCurrency)
			{
				int spaceLeftInStack = MAX_CURRENCY_STACK - GetItemAmount(existingCurrency);
				int amountToAdd = Math.Min(spaceLeftInStack, remainingAmount);

				existingCurrency.AddQuantity(amountToAdd);
				remainingAmount -= amountToAdd;

				//Print("[BattlegroundsRewards] Remaining currency added to players stack");
				if (remainingAmount <= 0) 
				{
					break;
				}
			}
			else 
			{
				//Print("[BattlegroundsRewards] Created a new stack of currency for player");
				int amountToCreate = Math.Clamp(remainingAmount, 1, MAX_CURRENCY_STACK);
				ItemBase moneyItem = ItemBase.Cast(player.GetInventory().CreateInInventory(classname));
				
				if (moneyItem)
				{
					moneyItem.SetQuantity(amountToCreate);
					remainingAmount -= amountToCreate;
				}
				else 
				{
					//Print("[BattlegroundsRewards] No space left to add currency to player");
					break;
				}
			}
		}
	}

	bool IsWearByPlayer(EntityAI entity)
	{
		InventoryLocation il = new InventoryLocation;
		entity.GetInventory().GetCurrentInventoryLocation(il);
		if (!il.IsValid())
			return true;

		// check the direct parent (clothing will be the parent if not on a man attachment)
		if (il.GetParent() && !il.GetParent().IsInherited(Man))
			return false;

		// items in hands are not worn
		if (il.GetType() == InventoryLocationType.HANDS)
			return false;

		return true;
	}

	// find an item (money) in the player's inventory by class name
	ItemBase FindItemInPlayerInventory(PlayerBase player, string classname)
	{
		classname.ToLower();
		array<EntityAI> itemsArray = new array<EntityAI> ;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);

		foreach(EntityAI entity: itemsArray)
		{
			if (entity.GetQuantity() == 100000)
				continue;	// skip stacks with quantity 100000

			string type = entity.GetType();
			type.ToLower();

			if (type == classname)
			{
				ItemBase temp = ItemBase.Cast(entity);
				if (temp && !temp.IsLockedInSlot() && !IsWearByPlayer(entity))
				{
					return temp;
				}
			}
		}

		return null;
	}

	int GetItemAmount(ItemBase item)
	{
		int quantity = 1;

		if (item.HasQuantity())
		{
			Magazine mag;
			if (Class.CastTo(mag, item))
			{
				quantity = mag.GetAmmoCount();
			}
			else
			{
				quantity = item.GetQuantity();
			}
		}
		else {}

		return quantity;
	}

	ItemBase FindItem(PlayerBase player, string classname)
	{
		ItemBase item = FindItemInPlayerInventory(player, classname);
		return item;
	}

	*/