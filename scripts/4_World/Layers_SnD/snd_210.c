class snd_210 extends SnD_Base  // bielawa farm
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy 210] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "2999.800049 448.002014 1132.827148";

        m_HumveePos = "3322.137939 233.941483 9710.349609";
        m_HumveeOrient = "-109.478119 -0.050684 -0.312779";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "2044.677612 253.354065 9503.565430";

        m_SiteAPos = "2096.477539 254.168747 9429.724609";
        m_SiteBPos = "1983.778076 252.489716 9526.281250";
    }
}