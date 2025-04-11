class BattlegroundsRewardManager
{
    private static ref BattlegroundsRewardManager m_Instance;
    protected ref CurrencyHelper m_CurrencyHelper;

    private void BattlegroundsRewardManager()
    { 
        m_CurrencyHelper = CurrencyHelper.GetInstance(); 
    }

    CurrencyHelper GetCurrencyHelper()
    { 
        return m_CurrencyHelper; 
    }

    static BattlegroundsRewardManager GetInstance()
    {
        if (!m_Instance)
            m_Instance = new BattlegroundsRewardManager();
            
        return m_Instance;
    }

    void UpdatePlayerStat(PlayerIdentity identity, string statName, int value)
    {
        string playerName = identity.GetName();
        DayZGame game = DayZGame.Cast(GetGame());
        BGPlayerStats playerStats = game.GetBattlegroundsConfig().FindPlayerStats(identity.GetId(), playerName);


        switch(statName)
        {
            case "zombieKills":
                playerStats.zombieKills += value;
                break;
            case "playerKills":
                playerStats.playerKills += value;
                break;
            case "playerDeaths":
                playerStats.deaths += value;
                break;
            case "cashEarned":
                playerStats.cashEarned += value;
                break;
        }

        playerStats.MarkAsChanged();
        BattlegroundsConfigLoader.MarkDataDirty();
    }
}