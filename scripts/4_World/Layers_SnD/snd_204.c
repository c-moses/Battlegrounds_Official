class snd_204 extends SnD_Base  // powerplant
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy 204] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "2999.800049 448.002014 1132.827148";

        m_HumveePos = "12552.146484 262.274689 7736.456055";
        m_HumveeOrient = "-137.050323 2.545556 0.284116";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "11555.826172 242.892517 7031.603516";

        m_SiteAPos = "11583.067383 243.404434 7126.596680";
        m_SiteBPos = "11516.236328 243.498230 6980.670898";
    }
}