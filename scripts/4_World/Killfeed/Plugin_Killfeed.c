class Killfeed extends PluginBase
{
	void Killfeed() {}

    void OnPlayerKilled(int deathType, PlayerBase victim = null, PlayerBase murder = null, EntityAI weapon = null)
    {
        string msg = GetKillPhrase(deathType);
        int type = 0;

        if (deathType == DeathType.PVP)
            type = 1;

        if (type == 1 && murder)
            SendKillInfo(murder, victim, type, weapon);
        else if (msg != "")
            SendKillInfo(null, victim, type, null, msg);
    }

	string GetKillPhrase(int deathType)
	{
		switch (deathType)
		{
			case DeathType.UNKNOWN:
				return "killed by an explosion";
			case DeathType.PVP:
				return "";
			case DeathType.SUICIDE:
				return "committed suicide";
			case DeathType.BLEEDING:
				return "bled out";
			case DeathType.STARVING:
				return "starved to death";
			case DeathType.ZOMBIE:
				return "killed by a zombie";
			case DeathType.ANIMAL:
				return "killed by an animal";
			case DeathType.FALL:
				return "fell to death";
			default:
				return "died in an unknown way";
		}
        return "died in an unknown way";
	}

	void SendKillInfo(PlayerBase murder, PlayerBase victim, int type, EntityAI weapon = null, string msg = "")
	{
		string murderName = "", targetName = "Unknown", murderWeaponType = "", message = msg;
		int dst = 0;
		PlayerBase recipient;
		array<Man> players;

		if (victim && victim.GetIdentity())
        {
			targetName = victim.GetIdentity().GetName();
            string targetTeam = "";
            if (victim.HasArmband())
            	targetTeam = victim.GetArmband();
        }

		// new
		string murderDistance = dst.ToString();
		string weaponName = weapon.GetDisplayName();
		if (type == 1 && murder)
		{
			murderName = murder.GetIdentity().GetName();
            string murderTeam = "";
            if (murder.HasArmband())
            	murderTeam = murder.GetArmband();

			dst = vector.Distance(victim.GetPosition(), murder.GetPosition());
			if (weapon)
				murderWeaponType = weapon.GetType();

			string rpcMessage = murderName + " killed " + targetName;
			if (weaponName != "")
			{
				string rpcMessageEdit = rpcMessage + " with " + weaponName;
				rpcMessage = rpcMessageEdit;
			}

			ServerRPC.SendServerRPC("RPC_KILLFEED", rpcMessage);
		}

		players = new array<Man>();
		GetGame().GetWorld().GetPlayerList(players);
		for (int i = 0; i < players.Count(); i++)
		{
            recipient = PlayerBase.Cast(players[i]);
			recipient.RPCSingleParam(-74700005, new Param8<string, string, string, string, string, int, string, int>(murderName, murderTeam, targetName, targetTeam, murderWeaponType, dst, message, type), true, recipient.GetIdentity());
		}
	}
}

Killfeed GetKillfeed()
{
	return Killfeed.Cast(GetPlugin(Killfeed));
}