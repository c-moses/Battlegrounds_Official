class CurrencyConfigLoader 
{
    protected static const string CONFIG_DIRECTORY = "$profile:\\Battlegrounds\\CurrencyData\\";

    static bool DoesPlayerCurrencyExist(string steamID)
    {
        string filePath = CONFIG_DIRECTORY + "User_" + steamID + ".json";
        return FileExist(filePath);
    }

    static PlayerCurrency GetOrCreatePlayerCurrency(string steamID, string playerName)
    {
        string filePath = CONFIG_DIRECTORY + "User_" + steamID + ".json";
        PlayerCurrency currency;

        if (FileExist(filePath)) {
            JsonFileLoader<PlayerCurrency>.JsonLoadFile(filePath, currency);
        } else {
            currency = CreateDefaultPlayerCurrency(steamID, playerName);
            SavePlayerCurrency(currency);
        }

        if (currency.PlayerName != playerName) {
            currency.PlayerName = playerName;
            SavePlayerCurrency(currency);
        }

        return currency;
    }

    static PlayerCurrency GetPlayerCurrency(string steamID)
    {
        string filePath = CONFIG_DIRECTORY + "User_" + steamID + ".json";
        PlayerCurrency currency;

        if (FileExist(filePath)) {
            JsonFileLoader<PlayerCurrency>.JsonLoadFile(filePath, currency);
        } else {
            currency = CreateDefaultPlayerCurrency(steamID, "");
            SavePlayerCurrency(currency);
        }

        return currency;
    }

    static PlayerCurrency CreateDefaultPlayerCurrency(string steamID, string playerName)
    {
        PlayerCurrency currency = new PlayerCurrency();
        currency.AccountID = steamID;
        currency.PlayerName = playerName;
        currency.Currency = 50000;
        return currency;
    }

    static void SavePlayerCurrency(PlayerCurrency currency)
    {
        if (!FileExist(CONFIG_DIRECTORY)) {
            MakeDirectory(CONFIG_DIRECTORY);
        }

        string filePath = CONFIG_DIRECTORY + "User_" + currency.AccountID + ".json";
        JsonFileLoader<PlayerCurrency>.JsonSaveFile(filePath, currency);
    }
}

class PlayerCurrency 
{
    string PlayerName;
    string AccountID;
    int Currency;

    void PlayerCurrency(string name = "", string accountId = "", int currency = 50000) 
    {
        PlayerName = name;
        AccountID = accountId;
        Currency = currency;
    }
}