class snd_107 extends SnD_Base  // vmc
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy v107] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "6897.090820 405.998260 11417.177734";

        m_HumveePos = "4727.504395 306.567108 7222.958008";
        m_HumveeOrient = "-66.236115 3.924219 -0.147225";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "4514.313965 316.249695 8282.795898";

        m_SiteAPos = "4545.433105 317.575653 8313.212891";
        m_SiteBPos = "4431.081543 313.089935 8233.541992";
    }
}
