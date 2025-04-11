class SnD_Site extends ItemBase
{
	void SnD_Site()
	{
		if (GetGame().IsServer())
			SetAllowDamage(false);
	}

    override bool CanPutInCargo(EntityAI parent)
	{
        return false;
    }
	
	override bool CanPutIntoHands(EntityAI parent)
	{
        return false;
    }

    override bool DisableVicinityIcon()
    {
        return true;
    }
};

class Offroad_SnD extends Offroad_02
{
	void SetupHumvee()
	{
		SpawnParts();
		FillUpCarFluids();

		GetInventory().CreateInInventory("Offroad_02_Wheel");
		GetInventory().CreateInInventory("Offroad_02_Wheel");
		GetInventory().CreateInInventory("Offroad_02_Wheel");
		GetInventory().CreateInInventory("Offroad_02_Wheel");
        GetInventory().CreateInInventory("Offroad_02_Wheel");

		GetInventory().CreateInInventory("Offroad_02_Door_1_1");
		GetInventory().CreateInInventory("Offroad_02_Door_1_2");
		GetInventory().CreateInInventory("Offroad_02_Door_2_1");
		GetInventory().CreateInInventory("Offroad_02_Door_2_2");
		GetInventory().CreateInInventory("Offroad_02_Hood");
		GetInventory().CreateInInventory("Offroad_02_Trunk");
	}

    void SpawnParts()
    {
        GetInventory().CreateInInventory("HeadlightH7");
		GetInventory().CreateInInventory("HeadlightH7");

        if (IsVitalCarBattery())
		    GetInventory().CreateInInventory("CarBattery");

        if (IsVitalGlowPlug())
		    GetInventory().CreateInInventory("GlowPlug");
    }
};