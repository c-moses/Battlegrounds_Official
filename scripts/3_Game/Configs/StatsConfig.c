class BattlegroundsConfig
{
    int configVersion;
	ref array< ref BGPlayerStats > Players;

    void BattlegroundsConfig()
    {
        configVersion = 1;
		Players = new ref array< ref BGPlayerStats >;
    }

    BGPlayerStats FindPlayerStats(string playerId, string playerName)
    {
        for (int i = 0; i < Players.Count(); i++)
        {
            if (Players[i].playerId == playerId)
            {
                if (Players[i].playerName != playerName)
                {
                    Players[i].playerName = playerName;
                    Players[i].MarkAsChanged();
                }
                return Players[i];
            }
        }

        BGPlayerStats newPlayer = new BGPlayerStats(playerId, playerName, 0, 0, 0, 0);
        Players.Insert(newPlayer);
        return newPlayer;
    }
}

class BattlegroundsConfigLoader {
    protected static const int CURRENT_CONFIG_VERSION = 1;
    protected static const string CONFIG_ROOT = "$profile:\\Battlegrounds\\";
    protected static const string CONFIG_FILE = CONFIG_ROOT + "Battlegrounds.json";
    protected static const string BACKUP_CONFIG_FILE = CONFIG_ROOT + "Battlegrounds_backup.json";
    protected static const string TEMP_CONFIG_FILE = CONFIG_ROOT + "Battlegrounds_temp.json";
    static bool m_DataDirty = false;
    static ref BattlegroundsConfig m_Config;
    protected static ref JsonSerializer m_Serializer = new JsonSerializer;

    static ref BattlegroundsConfig GetConfig() {
        if (!m_Config) {
            LoadConfig();
        }
        return m_Config;
    }

    static void LoadConfig() {
        string jsonData = LoadJsonFromFile(CONFIG_FILE);
        if (jsonData == "") {
            jsonData = LoadJsonFromFile(BACKUP_CONFIG_FILE);
        }
        if (jsonData != "") {
            Deserialize(jsonData);
        } else {
            CreateDefaultConfig();
        }
    }

    static string LoadJsonFromFile(string path) {
        FileHandle fileHandle = OpenFile(path, FileMode.READ);
        if (fileHandle == 0) {
            return "";
        }

        string line;
        string data = "";
        while (FGets(fileHandle, line) > 0) {
            data += line + "\n";
        }
        CloseFile(fileHandle);
        return data;
    }

    static void Deserialize(string jsonData) {
        string error;
        if (!m_Serializer.ReadFromString(m_Config, jsonData, error)) {
            CreateDefaultConfig();
        }
    }

    static void SaveConfig() 
    {
        if (!m_DataDirty || !m_Config) return;

        foreach (BGPlayerStats playerStats : m_Config.Players)
        {
            if (playerStats.IsDataChanged())
            {
                PlayerIdentity identity = GetIdentityByPlayerId(playerStats.playerId);
                if (identity)
                {
                    LevelManager levelManager = new LevelManager();
                    int playerLevel = levelManager.GetPlayerLevel(playerStats.cashEarned);
                    Param5<int, int, int, int, int> param = new Param5<int, int, int, int, int>(playerStats.playerKills, playerStats.zombieKills, playerStats.deaths, playerStats.cashEarned, playerLevel);

                    GetRPCManager().SendRPC("Battlegrounds", "ReceiveStats", param, true, identity);
                    playerStats.MarkAsSynced();
                }
            }
        }
        
        string jsonData;
        if (m_Serializer.WriteToString(m_Config, true, jsonData)) {
            if (SaveJsonToFile(TEMP_CONFIG_FILE, jsonData)) {
                if (!RenameFile(CONFIG_FILE, BACKUP_CONFIG_FILE)) {
                    SaveJsonToFile(BACKUP_CONFIG_FILE, jsonData);
                }

                if (!RenameFile(TEMP_CONFIG_FILE, CONFIG_FILE)) {
                    SaveJsonToFile(CONFIG_FILE, jsonData);
                } else {
                    m_DataDirty = false;
                }
            }
        }
    }

    static PlayerIdentity GetIdentityByPlayerId(string playerId)
    {
        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);
        for (int i = 0; i < players.Count(); i++)
        {
            PlayerIdentity identity = players[i].GetIdentity();
            if (identity && identity.GetId() == playerId)
            {
                return identity;
            }
        }

        return null;
    }

    static bool SaveJsonToFile(string path, string jsonData) {
        FileHandle fileHandle = OpenFile(path, FileMode.WRITE);
        if (fileHandle != 0) {
            FPrintln(fileHandle, jsonData);
            CloseFile(fileHandle);
            return true;
        } else {
            return false;
        }
    }

    static bool RenameFile(string originalPath, string newPath) {
        if (FileExist(originalPath)) {
            DeleteFile(newPath);
            CopyFile(originalPath, newPath);
            if (FileExist(newPath)) {
                DeleteFile(originalPath);
                return true;
            } else {
                return false;
            }
        }
        return false;
    }

    static void UpgradeConfig() {
        if (m_Config.configVersion < CURRENT_CONFIG_VERSION) {
            m_Config.configVersion = CURRENT_CONFIG_VERSION;
            MarkDataDirty();
        }
    }

    static void CreateDefaultConfig() {
        m_Config = new BattlegroundsConfig();
        MarkDataDirty();
    }

    static void MarkDataDirty() 
    { 
        m_DataDirty = true; 
    }
}

class BGPlayerStats
{
    string playerId;
	string playerName;
	int playerKills;
	int zombieKills;
	int deaths;
	int cashEarned;

	void BGPlayerStats(string id, string name, int pKills, int zKills, int deathCount, int cash)
	{
        playerId = id;
		playerName = name;
		playerKills = pKills;
		zombieKills = zKills;
		deaths = deathCount;
		cashEarned = cash;
	}

    bool dataChanged = false;

    void MarkAsChanged()
    { dataChanged = true; }

    void MarkAsSynced()
    { dataChanged = false; }

    bool IsDataChanged()
    { return dataChanged; }
}

modded class DayZGame
{	
	protected ref BattlegroundsConfig m_BattlegroundsConfig;
	protected ref TStringIntMap m_BGPlayerStats;
    protected float m_AccumulatedTime = 0.0;

    override void OnUpdate(bool doSim, float timeslice)
    {
        super.OnUpdate(doSim, timeslice);

        m_AccumulatedTime += timeslice;
        if (m_AccumulatedTime >= 59) // = seconds
        {
            SaveBattlegroundsData();
            m_AccumulatedTime = 0.0;
        }
    }

    void SaveBattlegroundsData()
    { BattlegroundsConfigLoader.SaveConfig(); }
	
	void SetBattlegroundsConfig(BattlegroundsConfig config)
    { m_BattlegroundsConfig = config; }

    BattlegroundsConfig GetBattlegroundsConfig()
    { return m_BattlegroundsConfig; }
	
	void SetBGPlayerStatsMap(BattlegroundsConfig config)
	{
		m_BGPlayerStats = new TStringIntMap;
		for (int i = 0; i < config.Players.Count(); i++)
		{ m_BGPlayerStats.Insert(config.Players.Get(i).playerName, config.Players.Get(i).cashEarned); }
	}

	int GetPlayerCash(string playerName)
	{
		int cash;
		bool found = m_BGPlayerStats.Find(playerName, cash);
		if (!found)
		{ return 0; }
		return cash;
	}
}

    /* Depracated ConfigLoader. New version utilizes serializer to read/write past 64kb .json limit
    
    protected static const int CURRENT_CONFIG_VERSION = 1;
	protected const static string m_ConfigRoot = "$profile:\\Battlegrounds\\";
    static bool m_DataDirty = false;

    static void MarkDataDirty()
    { m_DataDirty = true; }

    static bool LoadConfig(string configName, out BattlegroundsConfig configData)
    {
        string configPath = m_ConfigRoot + configName;
        string backupConfigPath = m_ConfigRoot + "Battlegrounds_backup.json";
        bool loadSuccessful = false;
        configData = null;
        if (FileExist(configPath))
        {
            JsonFileLoader<BattlegroundsConfig>.JsonLoadFile(configPath, configData);

            if (configData && configData.configVersion == CURRENT_CONFIG_VERSION) 
            { loadSuccessful = true; }
        }

        if (!loadSuccessful && FileExist(backupConfigPath))
        {
            JsonFileLoader<BattlegroundsConfig>.JsonLoadFile(backupConfigPath, configData);
            if (configData)
            { loadSuccessful = true; }
        }

        if (!loadSuccessful)
        {
            CreateDefaultConfig(configData);
            SaveConfig(configName, configData);
            if (configData)
            { loadSuccessful = true; }
        }
        return loadSuccessful;
    }

    static void SaveConfig(string configName, BattlegroundsConfig configData)
    {
        //Print("[BattlegroundsConfig] SaveConfig called");
        string configPath = m_ConfigRoot + configName;
        string tempConfigPath = m_ConfigRoot + configName + ".tmp";
        if(!m_DataDirty) return;
        if (!FileExist(m_ConfigRoot))
        {
            //Print("[BattlegroundsConfig] '" + m_ConfigRoot + "' does not exist, creating directory");
            MakeDirectory(m_ConfigRoot);
        }

        JsonFileLoader<BattlegroundsConfig>.JsonSaveFile(tempConfigPath, configData);

        if (FileExist(configPath))
        { RenameViaCopyDelete(configPath, m_ConfigRoot + "Battlegrounds_backup.json"); }

        RenameViaCopyDelete(tempConfigPath, configPath);
        m_DataDirty = false;
    }

    static void RenameViaCopyDelete(string originalPath, string newPath)
    {
        if (FileExist(originalPath))
        {
            CopyFile(originalPath, newPath);
            DeleteFile(originalPath);
        }
    }

    static void UpgradeConfig(ref BattlegroundsConfig configData)
    {
        if (configData.configVersion == 0)
        { configData.configVersion = 1; }
        SaveConfig("Battlegrounds.json", configData);
    }
	
    protected static void CreateDefaultConfig(out BattlegroundsConfig configData)
    {
        configData = new BattlegroundsConfig();
		configData.Players.Insert(new BGPlayerStats("Placeholder ID", "Placeholder Name", 0, 0, 0, 0));
	}

    */