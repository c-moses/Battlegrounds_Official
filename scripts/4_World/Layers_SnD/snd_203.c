class snd_203 extends SnD_Base  // zapadlisko
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy 203] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "2999.800049 448.002014 1132.827148";

        m_HumveePos = "7313.937988 217.481812 7954.842285";
        m_HumveeOrient = "54.249752 1.101705 -0.687971";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "8130.584473 245.177536 8730.208984";

        m_SiteAPos = "8138.686035 245.965698 8712.541992";
        m_SiteBPos = "7935.926270 265.002228 8725.660156";
    }
}
