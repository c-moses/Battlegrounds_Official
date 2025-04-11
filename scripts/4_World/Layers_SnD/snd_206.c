class snd_206 extends SnD_Base  // topolin
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy 206] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "2999.800049 448.002014 1132.827148";

        m_HumveePos = "562.275269 216.238937 8358.542969";
        m_HumveeOrient = "155.814072 3.437743 -0.047743";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "1515.310303 183.208191 7432.130859";

        m_SiteAPos = "1469.297607 183.642441 7384.361816";
        m_SiteBPos = "1514.975952 176.084045 7520.576660";
    }
}