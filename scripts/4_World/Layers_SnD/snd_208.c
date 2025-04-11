class snd_208 extends SnD_Base  // farm
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy 208] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "2999.800049 448.002014 1132.827148";

        m_HumveePos = "6168.473633 185.164566 11270.864258";
        m_HumveeOrient = "-132.697571 -0.886578 0.347800";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "5169.661133 193.780151 10421.241211";

        m_SiteAPos = "5227.903320 191.362457 10385.087891";
        m_SiteBPos = "5128.656738 195.331116 10456.129883";
    }
}

