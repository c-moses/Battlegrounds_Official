modded class MapMenu extends UIScriptedMenu
{
    private ref array<vector> m_BuyZones = new array<vector>;
    private vector m_ObjectiveLocation;
	private vector m_Objective2Location;
    private int m_ArmedObj = -1;

    void ClearEventMarkers()
    {
        m_MapWidgetInstance.ClearUserMarks();
    }

    void AddBuyZones(array<vector> buyZones)
    {
        m_BuyZones = buyZones;
        RefreshBuyZones();
    }

    void RefreshBuyZones()
    {
        if (!m_BuyZones || m_BuyZones.Count() == 0)
            return;

        foreach (vector zone : m_BuyZones)
        {
            m_MapWidgetInstance.AddUserMark(zone, "Buy Zone", GetMarkerColor("green"), MapMarkerTypes.GetMarkerTypeFromID(0));
        }
    }

    void AddEventPos(vector obj1, vector obj2, int armedObj)
    {
        m_ObjectiveLocation = obj1;
        m_Objective2Location = obj2;
        m_ArmedObj = armedObj;
        RefreshObjectives();
    }

    void RefreshObjectives()
    {
        if (m_ArmedObj == 1)
        {
            m_MapWidgetInstance.AddUserMark(m_ObjectiveLocation, "Armed Bomb", GetMarkerColor("red"), MapMarkerTypes.GetMarkerTypeFromID(0));
        }
        else if (m_ArmedObj == 2)
        {
            m_MapWidgetInstance.AddUserMark(m_Objective2Location, "Armed Bomb", GetMarkerColor("orange"), MapMarkerTypes.GetMarkerTypeFromID(0));
        }
        else
        {
            m_MapWidgetInstance.AddUserMark(m_ObjectiveLocation, "Objective", GetMarkerColor("red"), MapMarkerTypes.GetMarkerTypeFromID(0));

            if (m_Objective2Location != vector.Zero)
            {
                m_MapWidgetInstance.AddUserMark(m_Objective2Location, "Objective", GetMarkerColor("orange"), MapMarkerTypes.GetMarkerTypeFromID(0));
            }
        }
    }

    int GetMarkerColor(string color)
    {
        switch (color)
        {
            case "green":
                return ARGB(255, 40, 160, 70);

            case "red":
                return ARGB(255, 255, 0, 0);

            case "orange":
                return ARGB(255, 180, 90, 0);
        }

        return ARGB(255, 255, 255, 255);
    }
}