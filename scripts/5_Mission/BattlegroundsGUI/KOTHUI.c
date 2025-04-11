class KOTHWidget extends UIHelper
{
    protected ref Widget m_ZonePanel;
    protected TextWidget m_Location;
    protected TextWidget m_PlayerCount;
    protected ProgressBarWidget m_ProgressBar;
    protected bool m_IsVisible = false;

    override Widget Init()
    {
        layoutRoot          = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/KOTHWidget.layout");
        m_ZonePanel         = layoutRoot.FindAnyWidget("ZonePanel");
        m_Location          = TextWidget.Cast(layoutRoot.FindAnyWidget("Location"));
        m_PlayerCount       = TextWidget.Cast(layoutRoot.FindAnyWidget("PlayerCount"));
        m_ProgressBar       = ProgressBarWidget.Cast(layoutRoot.FindAnyWidget("ProgressBar"));
        SetID(UI_KOTH);
        return layoutRoot;
    }

    void CloseUI()
    {
        if (layoutRoot)
        {
            layoutRoot.Show(false);
            layoutRoot.Unlink();
        }

        GetGame().GetUIManager().HideScriptedMenu(this);
    }

    void ShowKOTH(bool show)
    {
        if (show)
        {
            layoutRoot.Show(true);
            m_IsVisible = true;
        }
        else
        {
            layoutRoot.Show(false);
            m_IsVisible = false;
        }
    }

    void ToggleKOTH()
    {
        if (m_IsVisible)
        {
            layoutRoot.Show(false);
            m_IsVisible = false;
        }
        else
        {
            layoutRoot.Show(true);
            m_IsVisible = true;
        }
    }

    void SetLocation(string location = "")
    {
        if (m_Location)
            m_Location.SetText(location);
    }

    void SetPlayerCount(int count = 0)
    {
        if (m_PlayerCount)
        {
            if (count == 0)
                m_PlayerCount.SetText("Active Event:");
            else if (count == 1)
                m_PlayerCount.SetText(count.ToString() + " Player at Event:");
            else
                m_PlayerCount.SetText(count.ToString() + " Players at Event:");
        }
    }

    void SetProgress(float progress = 0.0)
    {
        if (m_ProgressBar)
        {
            int roundedProgress = Math.Round(progress);
            m_ProgressBar.SetCurrent(roundedProgress);
        }
    }

    void SetDominantFaction(string dominantFaction)
    {
        if (m_ProgressBar)
            m_ProgressBar.SetColor((GetTeamARGB(dominantFaction)));
    }
}