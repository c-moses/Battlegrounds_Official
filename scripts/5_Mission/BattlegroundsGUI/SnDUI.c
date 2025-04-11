class SnDWidget extends UIHelper
{
    protected TextWidget m_BlueTeam;
    protected TextWidget m_RedTeam;
    protected bool m_IsVisible = false;

    override Widget Init()
    {
        layoutRoot          = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/SnDWidget.layout");
        m_BlueTeam          = TextWidget.Cast(layoutRoot.FindAnyWidget("BlueCount"));
        m_RedTeam           = TextWidget.Cast(layoutRoot.FindAnyWidget("RedCount"));
        SetID(UI_SND);
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

    void ShowSnD(bool show)
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

    void ToggleSnD()
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

    void SetPlayerCount(int blueTeam, int redTeam)
    {
        m_BlueTeam.SetText(blueTeam.ToString());
        m_RedTeam.SetText(redTeam.ToString());
    }
}