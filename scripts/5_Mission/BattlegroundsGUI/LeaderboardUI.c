class LeaderboardWidget extends UIHelper
{
    private GridSpacerWidget            m_CategoryGrid;
    private ButtonWidget                m_CloseLB;

    private ButtonWidget                m_OnlineCategory;
    private Widget                      m_OnlineFrame;
    private ScrollWidget                m_PlayerListOnline;
    private GridSpacerWidget            m_PlayerListGridOnline;

    private ButtonWidget                m_Top50Category;
    private Widget                      m_Top50Frame;
    private ScrollWidget                m_PlayerList;
    private GridSpacerWidget            m_PlayerListGrid;

    private ButtonWidget                m_PlayerCategory;
    private Widget 				        m_PlayerCat;
    private PlayerPreviewWidget         m_PlayerPreview;
    private TextWidget                  m_PlayerNameText;
    private TextWidget                  m_PlayerLevelText;
    private ProgressBarWidget           m_ProgressXP;
    private TextWidget                  m_XPBaseText;
    private TextWidget                  m_XPReqText;
    private TextWidget                  m_ScoreText;
    private TextWidget                  m_CashText;
    private TextWidget                  m_PKillsText;
    private TextWidget                  m_ZKillsText;
    private TextWidget                  m_DeathsText;
    private ButtonWidget                m_CopySteamID;
    private ButtonWidget                m_ShowSteamID;
    private TextWidget                  m_SteamIDText;
    private string                      m_SteamID;
    
    void LeaderboardWidget() {}
    void ~LeaderboardWidget()
    {
        PPEffects.SetBlurMenu(0);
        GetGame().GetUIManager().ShowUICursor(false);
        GetGame().GetMission().GetHud().Show(true);
        GetGame().GetMission().PlayerControlEnable(false);
        GetGame().GetInput().ResetGameFocus();
    }
    
    override Widget Init()
    {
        layoutRoot                  = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/LeaderboardWidget.layout");

        m_OnlineFrame               = Widget.Cast(layoutRoot.FindAnyWidget("OnlineFrame"));
        m_PlayerListOnline          = ScrollWidget.Cast(layoutRoot.FindAnyWidget("PlayerList2"));
        m_PlayerListGridOnline      = GridSpacerWidget.Cast(m_PlayerListOnline.FindAnyWidget("PlayerListGrid2"));

        m_Top50Frame                = Widget.Cast(layoutRoot.FindAnyWidget("Top50Frame"));
        m_PlayerList                = ScrollWidget.Cast(layoutRoot.FindAnyWidget("PlayerList"));
        m_PlayerListGrid            = GridSpacerWidget.Cast(m_PlayerList.FindAnyWidget("PlayerListGrid"));
        m_CategoryGrid              = GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("CategoryGrid"));
        m_CloseLB                   = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CloseButton"));

        m_PlayerCat                 = Widget.Cast(layoutRoot.FindAnyWidget("PlayerFrame"));
        m_PlayerPreview             = PlayerPreviewWidget.Cast(layoutRoot.FindAnyWidget("PlayerPreviewWidget"));
        m_PlayerNameText            = TextWidget.Cast(layoutRoot.FindAnyWidget("PlayerNameText"));
        m_PlayerLevelText           = TextWidget.Cast(layoutRoot.FindAnyWidget("XPLevelText"));
        m_ProgressXP                = ProgressBarWidget.Cast(layoutRoot.FindAnyWidget("XPProgressBar"));
        m_XPBaseText                = TextWidget.Cast(layoutRoot.FindAnyWidget("XPBaseText"));
        m_XPReqText                 = TextWidget.Cast(layoutRoot.FindAnyWidget("XPReqText"));
        m_ScoreText                 = TextWidget.Cast(layoutRoot.FindAnyWidget("ScoreText"));
        m_CashText                  = TextWidget.Cast(layoutRoot.FindAnyWidget("CashText"));
        m_PKillsText                = TextWidget.Cast(layoutRoot.FindAnyWidget("pKillsText"));
        m_ZKillsText                = TextWidget.Cast(layoutRoot.FindAnyWidget("zKillsText"));
        m_DeathsText                = TextWidget.Cast(layoutRoot.FindAnyWidget("DeathsText"));
        m_CopySteamID               = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CopySteamID"));
        m_ShowSteamID               = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ShowSteamID"));
        m_SteamIDText               = TextWidget.Cast(layoutRoot.FindAnyWidget("SteamIDText"));

        m_OnlineCategory     = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/CategoryButton2.layout", m_CategoryGrid));
        TextWidget.Cast(m_OnlineCategory.FindAnyWidget("CategoryText")).SetText("MATCH");
        m_OnlineCategory.SetUserID(69);

        m_Top50Category     = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/CategoryButton2.layout", m_CategoryGrid));
        TextWidget.Cast(m_Top50Category.FindAnyWidget("CategoryText")).SetText("TOP 50");
        m_Top50Category.SetUserID(69);

        m_PlayerCategory    = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/CategoryButton2.layout", m_CategoryGrid));
        TextWidget.Cast(m_PlayerCategory.FindAnyWidget("CategoryText")).SetText("PLAYER");
        m_PlayerCategory.SetUserID(69);

        m_PlayerListOnline.VScrollToPos01(0);
        m_PlayerList.VScrollToPos01(0);
        SetCategory(0);

        PPEffects.SetBlurMenu(0.3);
        GetGame().GetUIManager().ShowUICursor(true);
        GetGame().GetMission().GetHud().Show(false);
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
        GetGame().GetInput().ChangeGameFocus(1);
        SetID(MENU_LEADERBOARD);

        return layoutRoot;
    }

/////////////////// ON CLICK ///////////////////

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w == m_ShowSteamID)
        {
            if (m_ShowSteamID.GetState())
            {
                m_SteamIDText.SetText(m_SteamID);
            }
            else
            {
                m_SteamIDText.SetText("[SHOW STEAM ID]");
            }
            return true;
        }

        if (w == m_CopySteamID)
        {
            CopySteamIDToClipboard();
            return true;
        }

        if (w == m_OnlineCategory)
        {
            SetCategory(0);
            return true;
        }

        if (w == m_Top50Category)
        {
            SetCategory(1);
            return true;
        }

        if (w == m_PlayerCategory)
        {
            SetCategory(2);
            return true;
        }

        if (w == m_CloseLB)
        {
            Close();
            return true;
        }

        return false;
    }

/////////////////// ONLINE ///////////////////

    void ShowOnline()
    {
        m_PlayerCat.Show(false);
        m_Top50Frame.Show(false);
        m_OnlineFrame.Show(true);
    }

    void UpdatePlayerList(ref array<ref PlayerTeamData> playerListData, ref map<string, int> teamScores)
    {
        if (!playerListData || playerListData.Count() == 0)
            return;

        while (m_PlayerListGridOnline.GetChildren())
        {
            m_PlayerListGridOnline.RemoveChild(m_PlayerListGridOnline.GetChildren());
        }

        foreach (string teamID : ALL_TEAMS)
        {
            array<ref PlayerTeamData> teamPlayers = GetPlayersByTeam(playerListData, teamID);

            if (teamPlayers.Count() > 0)
            {
                int teamScore = 0;
                if (teamScores.Contains(teamID))
                    teamScore = teamScores.Get(teamID);

                int colorARGB = GetTeamARGB(teamID);
                AddTeamHeader(teamID, teamScore, colorARGB);
                foreach (PlayerTeamData player : teamPlayers)
                {
                    AddPlayerToOnlineList(player, colorARGB);
                }
            }
        }
    }

    void AddTeamHeader(string teamName, int teamScore, int colorARGB)
    {
        Widget teamEntry = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/LeaderboardEntryTeam.layout", m_PlayerListGridOnline);
        Widget outlineWidget = Widget.Cast(teamEntry.FindAnyWidget("Outline"));
        ImageWidget armbandWidget = ImageWidget.Cast(teamEntry.FindAnyWidget("Armband"));
        TextWidget teamNameWidget = TextWidget.Cast(teamEntry.FindAnyWidget("TeamName"));
        TextWidget teamScoreWidget = TextWidget.Cast(teamEntry.FindAnyWidget("TeamScore"));

        if (teamName != NO_TEAM)
        {
            armbandWidget.Show(true);
            armbandWidget.SetColor(colorARGB);
            outlineWidget.SetColor(colorARGB);
            teamScoreWidget.SetText("SCORE: " + teamScore.ToString());
        }

        string tName = GetTeamName(teamName);
        teamNameWidget.SetText(tName);
    }

    void AddPlayerToOnlineList(PlayerTeamData playerData, int colorARGB)
    {
        if (!playerData)
            return;

        Widget entryWidget = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/LeaderboardEntry2.layout", m_PlayerListGridOnline);

        ImageWidget deadWidget = ImageWidget.Cast(entryWidget.FindAnyWidget("Dead"));
        Widget playerOutlineWidget = Widget.Cast(entryWidget.FindAnyWidget("Outline"));
        TextWidget nameWidget = TextWidget.Cast(entryWidget.FindAnyWidget("PlayerName"));

        nameWidget.SetText(playerData.GetIdentity().GetName());
        playerOutlineWidget.SetColor(colorARGB);
        deadWidget.Show(!playerData.IsAlive());
    }

/////////////////// TOP 50 ///////////////////

    void ShowTop50()
    {
        m_OnlineFrame.Show(false);
        m_PlayerCat.Show(false);
        m_Top50Frame.Show(true);
    }

    void UpdateLeaderboardData(ref array<ref BGPlayerStats> leaderboardData)
    {
        if (!leaderboardData || leaderboardData.Count() == 0)
            return;

        while (m_PlayerListGrid.GetChildren())
        {
            m_PlayerListGrid.RemoveChild(m_PlayerListGrid.GetChildren());
        }

        for (int i = 0; i < leaderboardData.Count() && i < 50; i++)
        {
            AddPlayerToLeaderboard(i + 1, leaderboardData[i]);
        }
    }

    void AddPlayerToLeaderboard(int rank, BGPlayerStats playerStats)
    {
        if (!playerStats)
            return;

        Widget entryWidget = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/LeaderboardEntry.layout", m_PlayerListGrid);

        TextWidget rankWidget = TextWidget.Cast(entryWidget.FindAnyWidget("RankData"));
        TextWidget nameWidget = TextWidget.Cast(entryWidget.FindAnyWidget("NameData"));
        TextWidget pKillsWidget = TextWidget.Cast(entryWidget.FindAnyWidget("pKillsData"));
        TextWidget zKillsWidget = TextWidget.Cast(entryWidget.FindAnyWidget("zKillsData"));
        TextWidget deathsWidget = TextWidget.Cast(entryWidget.FindAnyWidget("DeathsData"));
        TextWidget scoreWidget = TextWidget.Cast(entryWidget.FindAnyWidget("ScoreData"));

        rankWidget.SetText(rank.ToString());
        nameWidget.SetText(playerStats.playerName);
        pKillsWidget.SetText(FormatNumberWithCommas(playerStats.playerKills));
        zKillsWidget.SetText(FormatNumberWithCommas(playerStats.zombieKills));
        deathsWidget.SetText(FormatNumberWithCommas(playerStats.deaths));
        scoreWidget.SetText(FormatNumberWithCommas(playerStats.cashEarned));
    }

/////////////////// PLAYER ///////////////////

    void ShowPlayer()
    {
        m_OnlineFrame.Show(false);
        m_Top50Frame.Show(false);
        m_PlayerCat.Show(true);
    }

    void SetPlayerStats(int level, int score, int cash, int pkills, int zkills, int deaths)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        m_SteamID = player.GetIdentity().GetPlainId();

        if (!m_PlayerNameText || !m_PlayerPreview)
            return;
        
        m_PlayerNameText.SetText(player.GetIdentity().GetName());

        m_PlayerPreview.SetPlayer(player);
        m_PlayerPreview.SetModelPosition(Vector(0, 0, 0.5));
        m_PlayerPreview.SetModelOrientation(Vector(0, 0, 0));
        m_PlayerPreview.SetSize(1, 1);
        m_PlayerPreview.Refresh();

        if (!m_PlayerLevelText || !m_XPBaseText || !m_XPReqText || !m_ProgressXP || !m_ScoreText || !m_PKillsText || !m_ZKillsText || !m_DeathsText)
            return;

        m_PlayerLevelText.SetText(level.ToString());
        LevelManager levelManager = new LevelManager();
        int xpForCurrentLevel = levelManager.GetXPForLevel(level);
        int xpForNextLevel = levelManager.GetXPForNextLevel(level);

        int xpProgress = score - xpForCurrentLevel;
        int xpRange = xpForNextLevel - xpForCurrentLevel;
        float progressPercentage = 0;

        if (xpRange > 0)
        {
            progressPercentage = (xpProgress / (float)xpRange) * 100;
        }

        m_XPBaseText.SetText(FormatNumberWithCommas(xpForCurrentLevel) + " XP");
        m_XPReqText.SetText(FormatNumberWithCommas(xpForNextLevel) + " XP");
        m_ProgressXP.SetCurrent(progressPercentage);

        m_ScoreText.SetText("SCORE: " + FormatNumberWithCommas(score) + " XP");
        m_CashText.SetText("CASH: $" + FormatNumberWithCommas(cash));
        m_PKillsText.SetText("PLAYER KILLS: " + FormatNumberWithCommas(pkills));
        m_ZKillsText.SetText("INFECTED KILLS: " + FormatNumberWithCommas(zkills));
        m_DeathsText.SetText("DEATHS: " + FormatNumberWithCommas(deaths));
    }

/////////////////// UTILITY ///////////////////

    void SetCategory(int categoryIndex)
    {
        Widget child = m_CategoryGrid.GetChildren();
        while (child)
        {
            ButtonWidget categoryButton = ButtonWidget.Cast(child);
            categoryButton.SetState(false);
            child = child.GetSibling();
        }

        if (categoryIndex == 0)
        { 
            m_OnlineCategory.SetState(true); 
            ShowOnline();
        }
        else if (categoryIndex == 1)
        { 
            m_Top50Category.SetState(true); 
            ShowTop50();
        }

        else if (categoryIndex == 2)
        { 
            m_PlayerCategory.SetState(true); 
            ShowPlayer();
        }
    }

    void CopySteamIDToClipboard()
    {
        if (m_SteamID && m_SteamID.Length() > 0)
        { 
            GetGame().CopyToClipboard(m_SteamID);
        }
    }

    string FormatNumberWithCommas(int number)
    {
        string numberStr = number.ToString();
        string formatted = "";
        int count = 0;
        for (int i = numberStr.Length() - 1; i >= 0; i--)
        {
            formatted = numberStr[i] + formatted;
            count++;
            if (count % 3 == 0 && i != 0)
                formatted = "," + formatted;
        }

        return formatted;
    }

    bool IsFocusable(Widget w)
    {
        if (w.GetUserID() == 69)
        { 
            return true; 
        }

        if (w == m_CloseLB)
        { 
            return true; 
        }

        return false;
    }

    void ColorNormal(Widget w)
    {
        if (!w)
            return;

        SetFocus(null);
    }

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		ColorNormal(w);
		return true;
	}
}