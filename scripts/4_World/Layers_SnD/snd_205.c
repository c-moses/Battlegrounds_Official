class snd_205 extends SnD_Base  // dolnik
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy 205] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "2999.800049 448.002014 1132.827148";

        m_HumveePos = "10657.018555 231.977966 1283.693237";
        m_HumveeOrient = "160.848785 -0.182035 0.258529";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "11485.442383 212.625610 453.413330";

        m_SiteAPos = "11535.125977 212.957794 397.727539";
        m_SiteBPos = "11449.108398 213.090118 517.424194";
    }
}