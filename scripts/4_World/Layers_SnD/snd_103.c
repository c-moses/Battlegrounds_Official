class snd_103 extends SnD_Base  // zeleno
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy v103] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "6897.090820 405.998260 11417.177734";

        m_HumveePos = "2524.163086 311.464417 6430.568848";
        m_HumveeOrient = "170.260880 -6.420569 1.224377";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "2493.718994 193.371765 5085.489746";

        m_SiteAPos = "2591.062012 195.057983 5091.329590";
        m_SiteBPos = "2486.318359 190.213348 5179.603516";
    }
}