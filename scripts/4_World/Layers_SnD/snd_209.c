class snd_209 extends SnD_Base  // swarog
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy 209] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "2999.800049 448.002014 1132.827148";

        m_HumveePos = "6177.298828 417.921265 2518.867920";
        m_HumveeOrient = "-108.325714 1.340269 2.354452";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "4956.160645 524.439270 2153.455078";

        m_SiteAPos = "4928.042969 529.028625 2195.658203";
        m_SiteBPos = "5022.463867 517.832458 2117.847900";
    }
}