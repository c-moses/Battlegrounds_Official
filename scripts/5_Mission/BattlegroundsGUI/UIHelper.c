class UIHelper extends UIScriptedMenu
{
    ref map<Widget, ref PreviewData> m_ItemPreviews = new map<Widget, ref PreviewData>();
    int m_SafezoneStatus = NO_ZONE;
    vector m_PlayerPosition = "0 0 0";

    void AddItemPreview(string itemType, Widget itemWidget)
    {
        if (m_ItemPreviews && m_ItemPreviews.Contains(itemWidget))
        {
            if (m_ItemPreviews[itemWidget].previewEntity)
                GetGame().ObjectDelete(m_ItemPreviews[itemWidget].previewEntity);
        }

        ItemPreviewWidget previewWidget = ItemPreviewWidget.Cast(itemWidget.FindAnyWidget("ItemPreview"));
        if (!previewWidget)
            return;

        EntityAI previewItem = EntityAI.Cast(GetGame().CreateObject(itemType, "0 0 0", true, false, true));
        if (!previewItem)
            return;

        previewWidget.SetItem(previewItem);
        previewWidget.SetModelPosition(Vector(0, 0, 0.5));
        previewWidget.SetModelOrientation(vector.Zero);
        previewWidget.SetView(previewItem.GetViewIndex());

        m_ItemPreviews.Insert(itemWidget, new PreviewData(previewWidget, previewItem));
    }

    void CleanupItemPreviews()
    {
        if (!m_ItemPreviews)
            return;

        foreach (Widget widget, PreviewData previewData : m_ItemPreviews)
        {
            if (previewData && previewData.previewEntity)
                GetGame().ObjectDelete(previewData.previewEntity);
        }

        m_ItemPreviews.Clear();
    }

    int GetSafezoneStatus()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return NO_ZONE;

        return player.GetSafezoneStatus();
    }

    vector GetPlayerPosition()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return vector.Zero;

        return player.GetPosition();
    }

    string FormatCurrency(int amount)
    {
        string amountStr = amount.ToString();
        string formatted = "";

        int count = 0;
        for (int i = amountStr.Length() - 1; i >= 0; i--)
        {
            formatted = amountStr[i] + formatted;
            count++;
            if (count % 3 == 0 && i != 0)
            {
                formatted = "," + formatted;
            }
        }

        return formatted;
    }

    ref array<string> ALL_TEAMS = { TEAM_BLUE, TEAM_RED, TEAM_GREEN, TEAM_ORANGE, TEAM_YELLOW, TEAM_PINK, NO_TEAM };
    array<ref PlayerTeamData> GetPlayersByTeam(ref array<ref PlayerTeamData> playerListData, string teamID)
    {
        array<ref PlayerTeamData> teamPlayers = new array<ref PlayerTeamData>();
        foreach (PlayerTeamData playerData : playerListData)
        {
            if (playerData && playerData.GetTeam() == teamID)
            {
                teamPlayers.Insert(playerData);
            }
        }
        return teamPlayers;
    }

    string GetTeamName(string teamID)
    {
        switch (teamID)
        {
            case TEAM_BLUE:
                return "TEAM BLUE";
            case TEAM_RED:
                return "TEAM RED";
            case TEAM_GREEN:
                return "TEAM GREEN";
            case TEAM_ORANGE:
                return "TEAM ORANGE";
            case TEAM_YELLOW:
                return "TEAM YELLOW";
            case TEAM_PINK:
                return "TEAM PINK";
            case NO_TEAM:
                return "-";
            default:
                return "";
        }
        return "";
    }

    int GetTeamARGB(string teamID)
    {
        switch (teamID)
        {
            case TEAM_BLUE:
                return ARGB(255, 48, 98, 173);
            case TEAM_RED:
                return ARGB(255, 157, 0, 0);
            case TEAM_GREEN:
                return ARGB(255, 0, 89, 0);
            case TEAM_ORANGE:
                return ARGB(255, 179, 89, 0);
            case TEAM_YELLOW:
                return ARGB(255, 133, 136, 0);
            case TEAM_PINK:
                return ARGB(255, 151, 70, 149);
            default:
                return ARGB(255, 255, 255, 255);
        }
        return ARGB(255, 255, 255, 255);
    }

    int GetTimerARGB(int time)
    {
        if (time<6)
            return ARGB(255, 255, 0, 0);
        else if (time<11)
            return ARGB(255, 255, 110, 0);
        else if (time<16)
            return ARGB(255, 255, 165, 0);
        else if (time<21)
            return ARGB(255, 255, 200, 0);
        else if (time<26)
            return ARGB(255, 255, 255, 0);
        else return ARGB(255, 255, 255, 255);
    }
}