class snd_104 extends SnD_Base  // severograd
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy v104] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "6897.090820 405.998260 11417.177734";

        m_HumveePos = "6898.290527 133.793701 12652.902344";
        m_HumveeOrient = "77.257568 -0.599993 0.385733";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "8013.452148 114.470917 12704.295898";

        m_SiteAPos = "8016.098633 117.950478 12755.172852";
        m_SiteBPos = "8003.742676 125.254898 12602.781250";
    }
}