class SnD_Bomb extends ExplosivesBase
{
    int netSync_BombState = -1;
    protected ref SnDManager m_SnDManager;
    string m_ArmedBy;

    void SnD_Bomb()
	{
		if (GetGame().IsServer())
			SetAllowDamage(false);

        SetParticleExplosion(ParticleList.PLASTIC_EXPLOSION);
	}

	override void InitItemVariables()
	{
		super.InitItemVariables();
        RegisterNetSyncVariableInt("netSync_BombState");
        m_SnDManager = SnDManager.GetInstance();
    }

    void SetBombState(int state)
    {
        netSync_BombState = state;
        SetSynchDirty();
    }

    int GetBombState()
    {
        return netSync_BombState;
    }

    void OnArmed(string team, string site)
    {
        if (GetBombState() == 0)
        {
            if (m_SnDManager)
                m_SnDManager.BombArmed(team, site);
        }

        m_ArmedBy = team;
    }

    string ArmedBy()
    {
        return m_ArmedBy;
    }

    void DetonateBomb()
    {
        InitiateExplosion();
    }

    void OnDisarmed(string team)
    {
        if (GetBombState() == 1)
        {
            if (m_SnDManager)
                m_SnDManager.BombDisarmed(team);
        }
    }

    override bool CanPutInCargo(EntityAI parent)
    {
        return GetBombState() == -1;
    }
    
    override bool CanPutIntoHands(EntityAI parent)
    {
        return GetBombState() == -1;
    }
};