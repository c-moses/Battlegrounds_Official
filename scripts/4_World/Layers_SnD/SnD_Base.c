class SnD_Base
{
    ref EventState m_EventState;
    
    string m_Map;

    protected vector m_MainSpawn;
    protected vector m_AttackSpawn;
    protected vector m_DefenseSpawn;
    protected vector m_SiteAPos;
    protected vector m_SiteBPos;
    protected vector m_HumveePos;
    protected vector m_HumveeOrient;

    protected SnD_Site m_SiteA;
    protected SnD_Site m_SiteB;
    protected Offroad_SnD m_Humvee1;
    protected Offroad_SnD m_Humvee2;
    protected SnD_Bomb m_Bomb;

    protected int m_LayerFlag;

    protected PunchedCard m_PunchedCard1;
    protected PunchedCard m_PunchedCard2;

// ================================= GAME MASTER =================================

    void CleanupLayer()
    {
        ClearExistingObjects();
        m_MainSpawn = vector.Zero;
        m_AttackSpawn = vector.Zero;
        m_DefenseSpawn = vector.Zero;
        m_SiteAPos = vector.Zero;
        m_SiteBPos = vector.Zero;
        m_HumveePos = vector.Zero;
        m_HumveeOrient = vector.Zero;
    }

// ================================= NEW =================================

    void CleanSpawnAreas()
    {
        float radius = 50.0;
        array<Object> nearbyObjects = new array<Object>();
        array<CargoBase> nearbyCargo = new array<CargoBase>();
        array<vector> sweepPositions = { m_AttackSpawn, m_DefenseSpawn };
        foreach (vector pos : sweepPositions)
        {
            nearbyObjects.Clear();

            GetGame().GetObjectsAtPosition3D(pos, radius, nearbyObjects, nearbyCargo);
            foreach (Object obj : nearbyObjects)
            {
                if (obj.IsInherited(ItemBase) || obj.IsInherited(Weapon_Base) || obj.IsInherited(Clothing_Base))
                {
                    GetGame().ObjectDelete(obj);
                }
                else
                {
                    PlayerBase player = PlayerBase.Cast(obj);
                    if (player && !player.IsAlive())
                    {
                        GetGame().ObjectDelete(player);
                    }
                }
            }
        }
    }

// ================================= - =================================

    void InitMap(string sndmap) 
    {
        m_EventState = EventState.GetInstance();
        m_LayerFlag = POS_STANDARD;
        m_Map = sndmap;
    }

    void ResetSnDRound()
    {
        ClearExistingObjects();
        SpawnBombSites();
        SpawnHumvees();
        SpawnBomb();
        SpawnExtras();
    }

    void EndSnDGame()
    {
        ClearExistingObjects();
    }

    void SpawnBombSites()
    {
        m_SiteA = SnD_Site.Cast(GetGame().CreateObject("SnD_SiteA", m_SiteAPos, false, true));
        m_SiteB = SnD_Site.Cast(GetGame().CreateObject("SnD_SiteB", m_SiteBPos, false, true));
    }

    void SpawnHumvees()
    {
        m_Humvee1 = Offroad_SnD.Cast(GetGame().CreateObject("Offroad_SnD", m_HumveePos, false, true));
        if (m_Humvee1)
        {
            m_Humvee1.SetOrientation(m_HumveeOrient);
            m_Humvee1.PlaceOnSurface();
            m_Humvee1.SetupHumvee();
        }

        vector direction = m_HumveeOrient.AnglesToVector();
        vector offsetPos = m_HumveePos + (direction * 10.0);

        m_Humvee2 = Offroad_SnD.Cast(GetGame().CreateObject("Offroad_SnD", offsetPos, false, true));
        if (m_Humvee2)
        {
            m_Humvee2.SetOrientation(m_HumveeOrient);
            m_Humvee2.PlaceOnSurface();
            m_Humvee2.SetupHumvee();
        }
    }

    void SpawnBomb()
    {
        if (m_Humvee1)
        {
            m_Bomb = SnD_Bomb.Cast(m_Humvee1.GetInventory().CreateInInventory("SnD_Bomb"));
        }
    }

    void SpawnExtras()
    {
        if (m_LayerFlag == POS_UNDERGROUND_ENOCH)
        {
            if (m_Humvee1)
            {
                m_PunchedCard1 = PunchedCard.Cast(m_Humvee1.GetInventory().CreateInInventory("PunchedCard"));
            }

            if (m_Humvee2)
            {
                m_PunchedCard2 = PunchedCard.Cast(m_Humvee1.GetInventory().CreateInInventory("PunchedCard"));
            }
        }
    }

    void DetonateBomb()
    {
        m_Bomb.DetonateBomb();
    }

    vector OffsetAttackPos(vector basePos, vector orient, float distance)
    {
        vector direction = orient.AnglesToVector();
        vector oppositeDirection = -direction;
        vector offsetPos = basePos + (oppositeDirection * distance);
        float surfaceY = GetGame().SurfaceY(offsetPos[0], offsetPos[2]);
        offsetPos[1] = surfaceY;

        return offsetPos;
    }

    vector GetAttackSpawn()
    {
        return m_AttackSpawn;
    }

    vector GetDefenseSpawn()
    {
        return m_DefenseSpawn;
    }

    void ClearExistingObjects()
    {
        if (m_SiteA)
        {
            GetGame().ObjectDelete(m_SiteA);
            m_SiteA = null;
        }

        if (m_SiteB)
        {
            GetGame().ObjectDelete(m_SiteB);
            m_SiteB = null;
        }

        if (m_Humvee1)
        {
            GetGame().ObjectDelete(m_Humvee1);
            m_Humvee1 = null;
        }

        if (m_Humvee2)
        {
            GetGame().ObjectDelete(m_Humvee2);
            m_Humvee2 = null;
        }

        if (m_Bomb)
        {
            GetGame().ObjectDelete(m_Bomb);
            m_Bomb = null;
        }

        if (m_LayerFlag == POS_UNDERGROUND_ENOCH)
        {
            if (m_PunchedCard1)
            {
                GetGame().ObjectDelete(m_PunchedCard1);
                m_PunchedCard1 = null;
            }

            if (m_PunchedCard2)
            {
                GetGame().ObjectDelete(m_PunchedCard2);
                m_PunchedCard2 = null;
            }
        }
    }
}