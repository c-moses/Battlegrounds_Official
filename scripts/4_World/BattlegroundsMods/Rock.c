class Rock extends Stone
{
	bool IsRockThrown;
	
	void Rock()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeleteRock, 5000, false);
	}
	
	void DeleteRock()
	{
		GetGame().ObjectDelete(this);
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
}