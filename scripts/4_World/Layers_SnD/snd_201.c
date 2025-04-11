class snd_201 extends SnD_Base  // livonia arms factory
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy 201] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "2999.800049 448.002014 1132.827148";

        m_HumveePos = "7862.670898 367.772186 3762.376709";
        m_HumveeOrient = "176.633224 -1.209145 0.283489";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "7435.885742 287.052429 2572.590576";

        m_SiteAPos = "7468.182129 287.423553 2522.751221";
        m_SiteBPos = "7347.921875 287.242676 2574.053467";
    }
}