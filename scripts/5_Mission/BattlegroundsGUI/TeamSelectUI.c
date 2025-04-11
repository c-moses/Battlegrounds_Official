class TeamSelectUI extends UIScriptedMenu
{
    private TextWidget m_Mode;
    private Widget m_TeamBlue;
    private Widget m_TeamRed;
    private Widget m_TeamGreen;
    private Widget m_TeamOrange;
    private Widget m_TeamYellow;
    private Widget m_TeamPink;
    private Widget m_NoTeam;
    private Widget m_Leaderboard;
    private TextWidget m_LeaderboardKey;

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/TeamSelectUI.layout");

        m_TeamBlue = Widget.Cast(layoutRoot.FindAnyWidget("BluePanel"));
        m_TeamRed = Widget.Cast(layoutRoot.FindAnyWidget("RedPanel"));
        m_TeamGreen = Widget.Cast(layoutRoot.FindAnyWidget("GreenPanel"));
        m_TeamOrange = Widget.Cast(layoutRoot.FindAnyWidget("OrangePanel"));
        m_TeamYellow = Widget.Cast(layoutRoot.FindAnyWidget("YellowPanel"));
        m_TeamPink = Widget.Cast(layoutRoot.FindAnyWidget("PinkPanel"));
        m_NoTeam = Widget.Cast(layoutRoot.FindAnyWidget("NoTeamPanel"));
        m_Leaderboard = Widget.Cast(layoutRoot.FindAnyWidget("LeaderboardPanel"));
        m_LeaderboardKey = TextWidget.Cast(layoutRoot.FindAnyWidget("ButtonText"));

        SetID(UI_TEAMSELECT);
        return layoutRoot;
    }

    void SetTeams(string mode)
    {
        if (mode == MODE_SND)
        {
            m_TeamBlue.Show(true);
            m_TeamRed.Show(true);
            LeaderboardToggle();
        }

        if (mode == MODE_KOTH)
        {
            m_TeamBlue.Show(true);
            m_TeamRed.Show(true);
            m_TeamGreen.Show(true);
            m_TeamOrange.Show(true);
            m_TeamYellow.Show(true);
            m_TeamPink.Show(true);
            m_NoTeam.Show(true);
            LeaderboardToggle();
        }
    }

    void LeaderboardToggle()
    {
        m_Leaderboard.Show(true);
        string lbKeyName = InputUtils.GetButtonNameFromInput("UALeaderboardToggle", EUAINPUT_DEVICE_KEYBOARD);
        m_LeaderboardKey.SetText(lbKeyName);
    }

    void CloseTeamSelect()
    {
        if (layoutRoot)
        {
            layoutRoot.Show(false);
            layoutRoot.Unlink();
        }

        GetGame().GetUIManager().HideScriptedMenu(this);
    }

    void HideTeams()
    {
        m_TeamBlue.Show(false);
        m_TeamRed.Show(false);
        m_TeamGreen.Show(false);
        m_TeamOrange.Show(false);
        m_TeamYellow.Show(false);
        m_TeamPink.Show(false);
        m_NoTeam.Show(false);
        m_Leaderboard.Show(false);
    }
}