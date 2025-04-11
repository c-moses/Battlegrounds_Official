class snd_106 extends SnD_Base  // novaya
{
    override void InitMap(string sndmap) 
    {
        super.InitMap(sndmap);

        SetEventPos();
        m_EventState.SetMainSpawn(m_MainSpawn);
        m_EventState.SetBlueSpawn(m_DefenseSpawn);
        m_EventState.SetRedSpawn(m_AttackSpawn);
        SnDManager.GetInstance().SetBombSitePos(m_SiteAPos, m_SiteBPos);

        Print("[Search & Destroy v106] Initialized");
    }

    void SetEventPos()
    {
        m_MainSpawn = "6897.090820 405.998260 11417.177734";

        m_HumveePos = "4006.062012 182.399246 13101.861328";
        m_HumveeOrient = "-90.736038 0.312678 -0.635669";

        m_AttackSpawn = OffsetAttackPos(m_HumveePos, m_HumveeOrient, 10.0);
        m_DefenseSpawn = "2988.655762 210.607162 12636.738281";

        m_SiteAPos = "2930.374023 220.808502 12644.530273";
        m_SiteBPos = "3082.579834 209.144104 12671.021484";
    }
}