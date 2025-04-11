class snd_101 extends SnD_Base  // nw airfield
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy v101] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "6897.090820 405.998260 11417.177734";

        m_HumveePos = "4232.476563 367.426483 11475.587891";
        m_HumveeOrient = "-140.803406 -8.955276 0.386174";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "4645.912598 338.974274 10387.666992";

        m_SiteAPos = "4630.258301 339.543457 10439.838867";
        m_SiteBPos = "4773.373047 339.217804 10317.747070";
    }
}