class snd_105 extends SnD_Base  // chernogorsk
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy v105] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "6897.090820 405.998260 11417.177734";

        m_HumveePos = "8351.876953 12.368835 3020.257568";
        m_HumveeOrient = "-58.337101 -2.309409 0.005487";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "7194.795898 6.658937 2958.350342";

        m_SiteAPos = "7258.168457 26.777132 3013.489502";
        m_SiteBPos = "7188.724609 11.662253 2869.807129";
    }
}