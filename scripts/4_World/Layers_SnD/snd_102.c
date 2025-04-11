class snd_102 extends SnD_Base  // tisy
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy v102] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "6897.090820 405.998260 11417.177734";

        m_HumveePos = "1588.628296 435.787231 14817.267578";
        m_HumveeOrient = "157.658737 0.295348 0.171454";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "1675.433228 451.742462 14273.947266";

        m_SiteAPos = "1683.110107 452.604065 14218.911133";
        m_SiteBPos = "1662.622070 451.748779 14349.631836";
    }
}