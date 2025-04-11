class snd_202 extends SnD_Base  // livonia bunker
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        m_EventState.SetResetPosFlag(POS_UNDERGROUND_ENOCH);
        m_LayerFlag = POS_UNDERGROUND_ENOCH;
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy 202] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "2999.800049 448.002014 1132.827148";

        m_HumveePos = "984.963928 592.293945 877.285522";
        m_HumveeOrient = "-85.540619 0.012648 -0.316376";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "649.266418 531.429871 1143.070435";

        m_SiteAPos = "717.159058 531.420288 1185.173828";
        m_SiteBPos = "601.412231 531.603821 1130.143433";
    }
}