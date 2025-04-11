class snd_207 extends SnD_Base  // sawmill
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy 207] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "2999.800049 448.002014 1132.827148";

        m_HumveePos = "10927.599609 174.203613 11111.535156";
        m_HumveeOrient = "157.199768 0.172316 -0.263018";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "11765.076172 173.225021 10716.004883";

        m_SiteAPos = "11776.282227 174.326172 10800.223633";
        m_SiteBPos = "11817.856445 173.709061 10659.282227";
    }
}