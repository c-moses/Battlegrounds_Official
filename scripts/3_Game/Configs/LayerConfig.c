//////////////////// GAME MASTER ////////////////////

class LayerInfo
{
    string layerCode;
    string layerName;

    void LayerInfo(string code, string name)
    {
        layerCode = code;
        layerName = name;
    }

    string GetKey()
    {
        return layerCode;
    }

    string GetName()
    {
        return layerName;
    }
}

class MapLayerConfig
{
    static ref array<ref LayerInfo> m_ChernarusplusLayers;
    static ref array<ref LayerInfo> m_EnochLayers;
    static ref array<ref LayerInfo> m_SakhalLayers;
    static ref array<ref LayerInfo> m_DeerIsleLayers;

    static ref array<string> GetVotableLayers(string world)
    {
        array<ref LayerInfo> allLayers;
        ref array<string> votableLayers = new array<string>();
        ref array<ref LayerInfo> kothLayers = new array<ref LayerInfo>();
        ref array<ref LayerInfo> sndLayers = new array<ref LayerInfo>();

        if (world == "chernarusplus")
            allLayers = GetChernarusplusLayers();
        else if (world == "enoch")
            allLayers = GetEnochLayers();
        else if (world == "sakhal")
            allLayers = GetSakhalLayers();
        else if (world == "deerisle")
            allLayers = GetDeerIsleLayers();
        else
            return votableLayers;

        foreach (LayerInfo layer : allLayers)
        {
            if (layer.GetKey().Contains("koth"))
            {
                kothLayers.Insert(layer);
            }
            else if (layer.GetKey().Contains("snd"))
            {
                sndLayers.Insert(layer);
            }
        }

        while (kothLayers.Count() > 0 && votableLayers.Count() < 2)
        {
            int randomIndex = Math.RandomInt(0, kothLayers.Count());
            votableLayers.Insert(kothLayers[randomIndex].GetKey());
            kothLayers.Remove(randomIndex);
        }

        while (sndLayers.Count() > 0 && votableLayers.Count() < 4)
        {
            int rndmIndex = Math.RandomInt(0, sndLayers.Count());
            votableLayers.Insert(sndLayers[rndmIndex].GetKey());
            sndLayers.Remove(rndmIndex);
        }

        return votableLayers;
    }

    static ref array<ref LayerInfo> GetChernarusplusLayers()
    {
        if (!m_ChernarusplusLayers)
        {
            m_ChernarusplusLayers = new array<ref LayerInfo>();
            m_ChernarusplusLayers.Insert(new LayerInfo("dayz_1", "Chernarus | Vanilla +"));
            m_ChernarusplusLayers.Insert(new LayerInfo("koth_101", "Chernarus | KOTH | Full map"));
            m_ChernarusplusLayers.Insert(new LayerInfo("koth_102", "Chernarus | KOTH | Chernogorsk"));
            m_ChernarusplusLayers.Insert(new LayerInfo("koth_103", "Chernarus | KOTH | Prison Island"));
            m_ChernarusplusLayers.Insert(new LayerInfo("koth_104", "Chernarus | KOTH | Northwest Airfield"));
            m_ChernarusplusLayers.Insert(new LayerInfo("koth_105", "Chernarus | KOTH | Novodmitrovsk"));
            m_ChernarusplusLayers.Insert(new LayerInfo("koth_106", "Chernarus | KOTH | Tisy Military"));
            m_ChernarusplusLayers.Insert(new LayerInfo("koth_107", "Chernarus | KOTH | Krasnostav"));
            m_ChernarusplusLayers.Insert(new LayerInfo("koth_108", "Chernarus | KOTH | Zelenogorsk"));
            m_ChernarusplusLayers.Insert(new LayerInfo("koth_109", "Chernarus | KOTH | Tri-Kresta/Krona Castle"));
            m_ChernarusplusLayers.Insert(new LayerInfo("koth_110", "Chernarus | KOTH | Berezino"));
            m_ChernarusplusLayers.Insert(new LayerInfo("koth_111", "Chernarus | KOTH | Vybor/Stary Sobor"));
            m_ChernarusplusLayers.Insert(new LayerInfo("koth_112", "Chernarus | KOTH | Pavlovo"));
            m_ChernarusplusLayers.Insert(new LayerInfo("koth_113", "Chernarus | KOTH | Elektrozavodsk"));
            m_ChernarusplusLayers.Insert(new LayerInfo("koth_114", "Chernarus | KOTH | Sinystok/Novaya"));
            m_ChernarusplusLayers.Insert(new LayerInfo("snd_101", "Chernarus | S&D | Northwest Airfield"));
            m_ChernarusplusLayers.Insert(new LayerInfo("snd_102", "Chernarus | S&D | Tisy Military"));
            m_ChernarusplusLayers.Insert(new LayerInfo("snd_103", "Chernarus | S&D | Zelenogorsk"));
            m_ChernarusplusLayers.Insert(new LayerInfo("snd_104", "Chernarus | S&D | Severograd"));
            m_ChernarusplusLayers.Insert(new LayerInfo("snd_105", "Chernarus | S&D | Chernogorsk"));
            m_ChernarusplusLayers.Insert(new LayerInfo("snd_106", "Chernarus | S&D | Novaya Petrovka"));
            m_ChernarusplusLayers.Insert(new LayerInfo("snd_107", "Chernarus | S&D | VMC"));
        }
        return m_ChernarusplusLayers;
    }

    static ref array<ref LayerInfo> GetEnochLayers()
    {
        if (!m_EnochLayers)
        {
            m_EnochLayers = new array<ref LayerInfo>();
            m_EnochLayers.Insert(new LayerInfo("dayz_2", "Livonia | Vanilla +"));
            m_EnochLayers.Insert(new LayerInfo("koth_201", "Livonia | KOTH | Full map"));
            m_EnochLayers.Insert(new LayerInfo("koth_202", "Livonia | KOTH | Topolin"));
            m_EnochLayers.Insert(new LayerInfo("koth_203", "Livonia | KOTH | Tarnow"));
            m_EnochLayers.Insert(new LayerInfo("koth_204", "Livonia | KOTH | Nadbor"));
            m_EnochLayers.Insert(new LayerInfo("koth_205", "Livonia | KOTH | Kopa Prison"));
            m_EnochLayers.Insert(new LayerInfo("koth_206", "Livonia | KOTH | Muratyn/Olszanka"));
            m_EnochLayers.Insert(new LayerInfo("snd_201", "Livonia | S&D | Arms Factory"));
            m_EnochLayers.Insert(new LayerInfo("snd_202", "Livonia | S&D | Dambog Bunker"));
            m_EnochLayers.Insert(new LayerInfo("snd_203", "Livonia | S&D | Zapadlisko"));
            m_EnochLayers.Insert(new LayerInfo("snd_204", "Livonia | S&D | Powerplant"));
            m_EnochLayers.Insert(new LayerInfo("snd_205", "Livonia | S&D | Dolnik"));
            m_EnochLayers.Insert(new LayerInfo("snd_206", "Livonia | S&D | Topolin"));
            m_EnochLayers.Insert(new LayerInfo("snd_207", "Livonia | S&D | Sawmill"));
            m_EnochLayers.Insert(new LayerInfo("snd_208", "Livonia | S&D | Gliniska Farm"));
            m_EnochLayers.Insert(new LayerInfo("snd_209", "Livonia | S&D | Swarog Military"));
            m_EnochLayers.Insert(new LayerInfo("snd_210", "Livonia | S&D | Bielawa Farm"));
        }
        return m_EnochLayers;
    }

    static ref array<ref LayerInfo> GetSakhalLayers()
    {
        if (!m_SakhalLayers)
        {
            m_SakhalLayers = new array<ref LayerInfo>();
            m_SakhalLayers.Insert(new LayerInfo("dayz_3", "Sakhal | Vanilla +"));
            m_SakhalLayers.Insert(new LayerInfo("koth_301", "Sakhal | KOTH | Full map"));
        }
        return m_SakhalLayers;
    }

    static ref array<ref LayerInfo> GetDeerIsleLayers()
    {
        if (!m_DeerIsleLayers)
        {
            m_DeerIsleLayers = new array<ref LayerInfo>();
            m_DeerIsleLayers.Insert(new LayerInfo("dayz_9", "Deer Isle | Vanilla +"));
        }
        return m_DeerIsleLayers;
    }

    static string GetWorldFromLayer(string layerKey)
    {
        array<ref LayerInfo> chernarusLayers = GetChernarusplusLayers();
        if (chernarusLayers)
        {
            foreach (LayerInfo chernaruslayer : chernarusLayers)
            {
                if (chernaruslayer.GetKey() == layerKey)
                    return "chernarusplus";
            }
        }

        array<ref LayerInfo> enochLayers = GetEnochLayers();
        if (enochLayers)
        {
            foreach (LayerInfo enochlayer : enochLayers)
            {
                if (enochlayer.GetKey() == layerKey)
                    return "enoch";
            }
        }

        array<ref LayerInfo> sakhalLayers = GetSakhalLayers();
        if (sakhalLayers)
        {
            foreach (LayerInfo sakhallayer : sakhalLayers)
            {
                if (sakhallayer.GetKey() == layerKey)
                    return "sakhal";
            }
        }

        array<ref LayerInfo> deerisleLayers = GetDeerIsleLayers();
        if (deerisleLayers)
        {
            foreach (LayerInfo deerislelayer : deerisleLayers)
            {
                if (deerislelayer.GetKey() == layerKey)
                    return "deerisle";
            }
        }

        return "";
    }

    static string GetLayerNameFromKey(string key)
    {
        array<ref LayerInfo> layers = new array<ref LayerInfo>();
        array<ref LayerInfo> chernarusLayers = MapLayerConfig.GetChernarusplusLayers();
        if (chernarusLayers)
        {
            foreach (LayerInfo chernaruslayer : chernarusLayers)
            {
                layers.Insert(chernaruslayer);
            }
        }

        array<ref LayerInfo> enochLayers = MapLayerConfig.GetEnochLayers();
        if (enochLayers)
        {
            foreach (LayerInfo enochlayer : enochLayers)
            {
                layers.Insert(enochlayer);
            }
        }

        array<ref LayerInfo> sakhalLayers = MapLayerConfig.GetSakhalLayers();
        if (sakhalLayers)
        {
            foreach (LayerInfo sakhallayer : sakhalLayers)
            {
                layers.Insert(sakhallayer);
            }
        }

        array<ref LayerInfo> deerisleLayers = MapLayerConfig.GetDeerIsleLayers();
        if (deerisleLayers)
        {
            foreach (LayerInfo deerislelayer : deerisleLayers)
            {
                layers.Insert(deerislelayer);
            }
        }

        foreach (LayerInfo layer : layers)
        {
            if (layer.GetKey() == key)
                return layer.GetName();
        }

        return "";
    }
}

//////////////////// JSON ////////////////////

class LayerConfig
{
    string LayerMode;
    int ScoreLimit;
    float RoundTime;
    float DefuseTime;
    float PreRoundTime;
    int RoundSwitch;
}

class LayerConfigLoader
{
    static void CheckDirectory(string directory)
    {
        if (!FileExist(directory))
        {
            MakeDirectory(directory);
        }
    }

    static bool LoadConfig(string filename, out LayerConfig config)
    {
        if (FileExist(filename))
        {
            JsonFileLoader<LayerConfig>.JsonLoadFile(filename, config);
            return true;
        }
        return false;
    }

    static void CreateDefaultConfig(string filename)
    {
        LayerConfig config = new LayerConfig();

        config.LayerMode = "dayz_1";
        config.ScoreLimit = 10;
        config.RoundTime = 600.0;
        config.DefuseTime = 90.0;
        config.PreRoundTime = 90.0;
        config.RoundSwitch = 3;

        JsonFileLoader<LayerConfig>.JsonSaveFile(filename, config);
    }

    static string GetMode(string mapVersion)
    {
        int underscoreIndex = mapVersion.IndexOf("_");
        if (underscoreIndex == -1)
        {
            return "";
        }

        return mapVersion.Substring(0, underscoreIndex);
    }

    static string GetVersion(string mapVersion)
    {
        int underscoreIndex = mapVersion.IndexOf("_");
        if (underscoreIndex == -1)
        {
            return "";
        }

        int len = mapVersion.Length() - (underscoreIndex + 1);
        return mapVersion.Substring(underscoreIndex + 1, len);
    }

    static void UpdateLayerConfig(string filename, string newLayerMode, float roundTime, int scoreLimit, float bombTime, float preRoundTime, int roundSwitch)
    {
        LayerConfig config;
        if (!LoadConfig(filename, config))
        {
            config = new LayerConfig();
            CreateDefaultConfig(filename);
        }

        config.LayerMode = newLayerMode;
        config.RoundTime = roundTime;
        config.ScoreLimit = scoreLimit;
        config.DefuseTime = bombTime;
        config.PreRoundTime = preRoundTime;
        config.RoundSwitch = roundSwitch;

        JsonFileLoader<LayerConfig>.JsonSaveFile(filename, config);
    }
}