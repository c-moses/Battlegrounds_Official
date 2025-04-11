class PreviewData
{
    ItemPreviewWidget previewWidget;
    EntityAI previewEntity;

    void PreviewData(ItemPreviewWidget widget, EntityAI entity)
    {
        previewWidget = widget;
        previewEntity = entity;
    }
};

class ItemHelper
{
    static string GetDisplayName(string classname)
    {
        string displayName = "";
        if (GetGame().ConfigGetText(CFG_VEHICLESPATH + " " + classname + " displayName", displayName))
        {
            displayName = TrimUnt(displayName);
            return displayName;
        }

        if (GetGame().ConfigGetText(CFG_WEAPONSPATH + " " + classname + " displayName", displayName))
        {
            displayName = TrimUnt(displayName);
            return displayName;
        }

        if (GetGame().ConfigGetText(CFG_MAGAZINESPATH + " " + classname + " displayName", displayName))
        {
            displayName = TrimUnt(displayName);
            return displayName;
        }

        return classname;
    }
    
    static string TrimUnt(string input)
    {
        int index = input.IndexOf("$UNT$");
        if (index != -1)
        {
            string firstPart = input.Substring(0, index);
            string secondPart = input.Substring(index + 5, input.Length() - (index + 5));
            input = firstPart + secondPart;
        }
        return input;
    }
};

class SpawnConfig
{
    string category;  // character, torso, legs, head, hands, feet
    string classname; // item classname
    int levelRequirement; // required level
    string relatedItem; // related mag/ammo type, can determine by prefix

    void SpawnConfig(string cat, string clss, int Requirement, string Related)
    {
        category = cat;
        classname = clss;
        levelRequirement = Requirement;
        relatedItem = Related;
    }
};

class SpawnMenuItems
{
    static ref array<ref SpawnConfig> Character()
    {
        return {
            new SpawnConfig("Character", "SurvivorM_Mirek", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Boris", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Cyril", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Denis", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Elias", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Francis", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Guo", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Hassan", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Indar", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Jose", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Kaito", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Lewis", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Manua", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Niki", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Oliver", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Peter", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Quinn", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Rolf", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Seth", 0, ""),
            new SpawnConfig("Character", "SurvivorM_Taiki", 0, ""),
            new SpawnConfig("Character", "SurvivorF_Eva", 0, ""),
            new SpawnConfig("Character", "SurvivorF_Frida", 0, ""),
            new SpawnConfig("Character", "SurvivorF_Gabi", 0, ""),
            new SpawnConfig("Character", "SurvivorF_Helga", 0, ""),
            new SpawnConfig("Character", "SurvivorF_Irena", 0, ""),
            new SpawnConfig("Character", "SurvivorF_Judy", 0, ""),
            new SpawnConfig("Character", "SurvivorF_Keiko", 0, ""),
            new SpawnConfig("Character", "SurvivorF_Linda", 0, ""),
            new SpawnConfig("Character", "SurvivorF_Maria", 0, ""),
            new SpawnConfig("Character", "SurvivorF_Naomi", 0, ""),
            new SpawnConfig("Character", "SurvivorF_Baty", 0, "")
        };
    }

    static ref array<ref SpawnConfig> Sidearm()
    {
        return {
            new SpawnConfig("set:dayz_inventory image:pistol", "MakarovIJ70", 0, "Mag_IJ70_8Rnd"),
            new SpawnConfig("set:dayz_inventory image:pistol", "P1", 5, "Mag_P1_8Rnd"),
            new SpawnConfig("set:dayz_inventory image:pistol", "Colt1911", 10, "Mag_1911_7Rnd"),
            new SpawnConfig("set:dayz_inventory image:pistol", "Magnum", 15, "Ammo_357"),
            new SpawnConfig("set:dayz_inventory image:pistol", "Engraved1911", 20, "Mag_1911_7Rnd"),
            new SpawnConfig("set:dayz_inventory image:pistol", "CZ75", 25, "Mag_CZ75_15Rnd"),
            new SpawnConfig("set:dayz_inventory image:pistol", "Glock19", 30, "Mag_Glock_15Rnd"),
            new SpawnConfig("set:dayz_inventory image:pistol", "FNX45", 35, "Mag_FNX45_15Rnd"),
            new SpawnConfig("set:dayz_inventory image:pistol", "Deagle", 40, "Mag_Deagle_9rnd"),
            new SpawnConfig("set:dayz_inventory image:pistol", "CZ61", 45, "Mag_CZ61_20Rnd"),
            new SpawnConfig("set:dayz_inventory image:pistol", "Deagle_Gold", 50, "Mag_Deagle_9rnd"),
            new SpawnConfig("set:dayz_inventory image:pistol", "LongHorn", 60, "Ammo_308Win")
        };
    }

    static ref array<ref SpawnConfig> Torso()
    {
        return {
            new SpawnConfig("set:dayz_inventory image:body", "TShirt_Black", 0, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TShirt_Grey", 0, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TShirt_RedBlackStripes", 0, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TShirt_Red", 0, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TShirt_Blue", 0, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TShirt_Beige", 0, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TShirt_Green", 0, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TShirt_OrangeWhiteStripes", 0, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TShirt_White", 0, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TShirt_10thAnniversary", 1, ""),
            new SpawnConfig("set:dayz_inventory image:body", "ChernarusSportShirt", 1, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TelnyashkaShirt", 1, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Blouse_White", 1, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Blouse_Green", 1, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Blouse_Blue", 1, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Blouse_Violet", 1, ""),
            new SpawnConfig("set:dayz_inventory image:body", "MedicalScrubsShirt_White", 2, ""),
            new SpawnConfig("set:dayz_inventory image:body", "MedicalScrubsShirt_Blue", 2, ""),
            new SpawnConfig("set:dayz_inventory image:body", "MedicalScrubsShirt_Green", 2, ""),
            new SpawnConfig("set:dayz_inventory image:body", "PrisonUniformJacket", 3, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Sweater_Gray", 4, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Sweater_Green", 4, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Sweater_Blue", 4, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Sweater_Red", 4, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Shirt_PlaneBlack", 7, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Shirt_GreenCheck", 7, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Shirt_BlueCheck", 7, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Shirt_BlueCheckBright", 7, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Shirt_RedCheck", 7, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Shirt_WhiteCheck", 7, ""),
            new SpawnConfig("set:dayz_inventory image:body", "ManSuit_Black", 8, ""),
            new SpawnConfig("set:dayz_inventory image:body", "ManSuit_Brown", 8, ""),
            new SpawnConfig("set:dayz_inventory image:body", "ManSuit_DarkGrey", 8, ""),
            new SpawnConfig("set:dayz_inventory image:body", "ManSuit_LightGrey", 8, ""),
            new SpawnConfig("set:dayz_inventory image:body", "ManSuit_Blue", 8, ""),
            new SpawnConfig("set:dayz_inventory image:body", "ManSuit_Khaki", 8, ""),
            new SpawnConfig("set:dayz_inventory image:body", "ManSuit_Beige", 8, ""),
            new SpawnConfig("set:dayz_inventory image:body", "ManSuit_White", 8, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Raincoat_Black", 9, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Raincoat_Green", 9, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Raincoat_Blue", 9, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Raincoat_Yellow", 9, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Raincoat_Orange", 9, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Raincoat_Red", 9, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Raincoat_Pink", 9, ""),
            new SpawnConfig("set:dayz_inventory image:body", "DenimJacket", 10, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Hoodie_Black", 11, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Hoodie_Brown", 11, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Hoodie_Grey", 11, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Hoodie_Green", 11, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Hoodie_Blue", 11, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Hoodie_Red", 11, ""),
            new SpawnConfig("set:dayz_inventory image:body", "JumpsuitJacket_Gray", 12, ""),
            new SpawnConfig("set:dayz_inventory image:body", "JumpsuitJacket_Blue", 12, ""),
            new SpawnConfig("set:dayz_inventory image:body", "JumpsuitJacket_Green", 12, ""),
            new SpawnConfig("set:dayz_inventory image:body", "JumpsuitJacket_Red", 12, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TrackSuitJacket_Black", 14, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TrackSuitJacket_Blue", 14, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TrackSuitJacket_LightBlue", 14, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TrackSuitJacket_Green", 14, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TrackSuitJacket_Red", 14, ""),
            new SpawnConfig("set:dayz_inventory image:body", "ParamedicJacket_Blue", 16, ""),
            new SpawnConfig("set:dayz_inventory image:body", "ParamedicJacket_Green", 16, ""),
            new SpawnConfig("set:dayz_inventory image:body", "ParamedicJacket_Crimson", 16, ""),
            new SpawnConfig("set:dayz_inventory image:body", "HikingJacket_Black", 17, ""),
            new SpawnConfig("set:dayz_inventory image:body", "HikingJacket_Blue", 17, ""),
            new SpawnConfig("set:dayz_inventory image:body", "HikingJacket_Green", 17, ""),
            new SpawnConfig("set:dayz_inventory image:body", "HikingJacket_Red", 17, ""),
            new SpawnConfig("set:dayz_inventory image:body", "PoliceJacket", 18, ""),
            new SpawnConfig("set:dayz_inventory image:body", "PoliceJacketOrel", 18, ""),
            new SpawnConfig("set:dayz_inventory image:body", "BomberJacket_Black", 19, ""),
            new SpawnConfig("set:dayz_inventory image:body", "BomberJacket_Brown", 19, ""),
            new SpawnConfig("set:dayz_inventory image:body", "BomberJacket_Blue", 19, ""),
            new SpawnConfig("set:dayz_inventory image:body", "BomberJacket_SkyBlue", 19, ""),
            new SpawnConfig("set:dayz_inventory image:body", "BomberJacket_Grey", 19, ""),
            new SpawnConfig("set:dayz_inventory image:body", "BomberJacket_Olive", 19, ""),
            new SpawnConfig("set:dayz_inventory image:body", "BomberJacket_Maroon", 19, ""),
            new SpawnConfig("set:dayz_inventory image:body", "NBCJacketGray", 20, ""),
            new SpawnConfig("set:dayz_inventory image:body", "NBCJacketYellow", 20, ""),
            new SpawnConfig("set:dayz_inventory image:body", "WoolCoat_Black", 21, ""),
            new SpawnConfig("set:dayz_inventory image:body", "WoolCoat_BlackCheck", 21, ""),
            new SpawnConfig("set:dayz_inventory image:body", "WoolCoat_BrownCheck", 21, ""),
            new SpawnConfig("set:dayz_inventory image:body", "WoolCoat_BlueCheck", 21, ""),
            new SpawnConfig("set:dayz_inventory image:body", "WoolCoat_Blue", 21, ""),
            new SpawnConfig("set:dayz_inventory image:body", "WoolCoat_Green", 21, ""),
            new SpawnConfig("set:dayz_inventory image:body", "WoolCoat_Beige", 21, ""),
            new SpawnConfig("set:dayz_inventory image:body", "WoolCoat_GreyCheck", 21, ""),
            new SpawnConfig("set:dayz_inventory image:body", "WoolCoat_Red", 21, ""),
            new SpawnConfig("set:dayz_inventory image:body", "RidersJacket_Black", 22, ""),
            new SpawnConfig("set:dayz_inventory image:body", "RidersJacket_Black", 22, ""),
            new SpawnConfig("set:dayz_inventory image:body", "NavyUniformJacket", 23, ""),
            new SpawnConfig("set:dayz_inventory image:body", "FirefighterJacket_Beige", 24, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TacticalShirt_Black", 25, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TacticalShirt_Olive", 25, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TacticalShirt_Tan", 25, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TacticalShirt_Grey", 25, ""),
            new SpawnConfig("set:dayz_inventory image:body", "HuntingJacket_Winter", 26, ""),
            new SpawnConfig("set:dayz_inventory image:body", "HuntingJacket_Brown", 26, ""),
            new SpawnConfig("set:dayz_inventory image:body", "HuntingJacket_Summer", 26, ""),
            new SpawnConfig("set:dayz_inventory image:body", "HuntingJacket_Autumn", 26, ""),
            new SpawnConfig("set:dayz_inventory image:body", "HuntingJacket_Spring", 26, ""),
            new SpawnConfig("set:dayz_inventory image:body", "QuiltedJacket_Black", 28, ""),
            new SpawnConfig("set:dayz_inventory image:body", "QuiltedJacket_Blue", 28, ""),
            new SpawnConfig("set:dayz_inventory image:body", "QuiltedJacket_Violet", 28, ""),
            new SpawnConfig("set:dayz_inventory image:body", "QuiltedJacket_Green", 28, ""),
            new SpawnConfig("set:dayz_inventory image:body", "QuiltedJacket_Grey", 28, ""),
            new SpawnConfig("set:dayz_inventory image:body", "QuiltedJacket_Yellow", 28, ""),
            new SpawnConfig("set:dayz_inventory image:body", "QuiltedJacket_Orange", 28, ""),
            new SpawnConfig("set:dayz_inventory image:body", "QuiltedJacket_Red", 28, ""),
            new SpawnConfig("set:dayz_inventory image:body", "DownJacket_Blue", 29, ""),
            new SpawnConfig("set:dayz_inventory image:body", "DownJacket_Green", 29, ""),
            new SpawnConfig("set:dayz_inventory image:body", "DownJacket_Orange", 29, ""),
            new SpawnConfig("set:dayz_inventory image:body", "DownJacket_Red", 29, ""),
            new SpawnConfig("set:dayz_inventory image:body", "GorkaEJacket_Winter", 30, ""),
            new SpawnConfig("set:dayz_inventory image:body", "GorkaEJacket_PautRev", 30, ""),
            new SpawnConfig("set:dayz_inventory image:body", "GorkaEJacket_Autumn", 30, ""),
            new SpawnConfig("set:dayz_inventory image:body", "GorkaEJacket_Summer", 30, ""),
            new SpawnConfig("set:dayz_inventory image:body", "GorkaEJacket_Flat", 30, ""),
            new SpawnConfig("set:dayz_inventory image:body", "BushlatPoliceJacket_Blue", 32, ""),
            new SpawnConfig("set:dayz_inventory image:body", "OMKJacket_Navy", 35, ""),
            new SpawnConfig("set:dayz_inventory image:body", "USMCJacket_Woodland", 35, ""),
            new SpawnConfig("set:dayz_inventory image:body", "USMCJacket_Desert", 35, ""),
            new SpawnConfig("set:dayz_inventory image:body", "TTsKOJacket_Camo", 40, ""),
            new SpawnConfig("set:dayz_inventory image:body", "BDUJacket", 45, ""),
            new SpawnConfig("set:dayz_inventory image:body", "M65Jacket_Black", 50, ""),
            new SpawnConfig("set:dayz_inventory image:body", "M65Jacket_Khaki", 50, ""),
            new SpawnConfig("set:dayz_inventory image:body", "M65Jacket_Tan", 50, ""),
            new SpawnConfig("set:dayz_inventory image:body", "Chainmail", 50, "")
        };
    }
    static ref array<ref SpawnConfig> Legs()
    {
        return {
            new SpawnConfig("set:dayz_inventory image:legs", "CanvasPantsMidi_Grey", 0, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "CanvasPantsMidi_Violet", 0, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "CanvasPantsMidi_Blue", 0, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "CanvasPantsMidi_Beige", 0, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "CanvasPantsMidi_Red", 0, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "MedicalScrubsPants_White", 2, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "MedicalScrubsPants_Blue", 2, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "MedicalScrubsPants_Green", 2, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "PrisonUniformPants", 3, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "ShortJeans_Black", 4, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "ShortJeans_Brown", 4, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "ShortJeans_Darkblue", 4, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "ShortJeans_Blue", 4, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "ShortJeans_Green", 4, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "ShortJeans_Red", 4, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Skirt_White", 4, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Skirt_Blue", 4, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Skirt_Red", 4, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Skirt_Yellow", 4, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Breeches_Black", 5, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Breeches_Blackcheck", 5, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Breeches_Beetcheck", 5, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Breeches_Browncheck", 5, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Breeches_Blue", 5, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Breeches_Green", 5, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Breeches_Beige", 5, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Breeches_White", 5, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Breeches_Pink", 5, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Breeches_Red", 5, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "SlacksPants_Black", 8, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "SlacksPants_Brown", 8, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "SlacksPants_Blue", 8, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "SlacksPants_DarkGrey", 8, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "SlacksPants_Khaki", 8, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "SlacksPants_LightGrey", 8, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "SlacksPants_White", 8, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "JumpsuitPants_Grey", 10, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "JumpsuitPants_Blue", 10, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "JumpsuitPants_Green", 10, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "JumpsuitPants_Red", 10, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Jeans_Black", 12, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Jeans_Brown", 12, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Jeans_Grey", 12, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Jeans_BlueDark", 12, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Jeans_Blue", 12, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Jeans_Green", 12, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "TrackSuitPants_Black", 14, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "TrackSuitPants_Blue", 14, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "TrackSuitPants_LightBlue", 14, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "TrackSuitPants_Green", 14, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "TrackSuitPants_Red", 14, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "ParamedicPants_Blue", 16, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "ParamedicPants_Green", 16, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "ParamedicPants_Crimson", 16, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "PolicePants", 18, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "PolicePantsOrel", 18, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "NBCPantsGray", 20, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "NBCPantsYellow", 20, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "CargoPants_Black", 22, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "CargoPants_Blue", 22, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "CargoPants_Grey", 22, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "CargoPants_Green", 22, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "CargoPants_Beige", 22, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "NavyUniformPants", 23, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "FirefightersPants_Black", 24, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "FirefightersPants_Beige", 24, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "HunterPants_Winter", 26, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "HunterPants_Brown", 26, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "HunterPants_Summer", 26, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "HunterPants_Autumn", 26, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "HunterPants_Spring", 26, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "GorkaPants_Winter", 30, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "GorkaPants_PautRev", 30, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "GorkaPants_Autumn", 30, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "GorkaPants_Summer", 30, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "GorkaPants_Flat", 30, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "OMKPants_Navy", 35, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "USMCPants_Woodland", 35, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "USMCPants_Desert", 35, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "TTSKOPants", 40, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "BDUPants", 45, ""),
            new SpawnConfig("set:dayz_inventory image:legs", "Chainmail_Leggings", 50, "")
        };
    }
    static ref array<ref SpawnConfig> Feet()
    {
        return {
            new SpawnConfig("set:dayz_inventory image:feet", "AthleticShoes_Black", 0, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "AthleticShoes_Grey", 0, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "AthleticShoes_Brown", 0, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "AthleticShoes_Green", 0, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "AthleticShoes_Blue", 0, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "JoggingShoes_Black", 1, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "JoggingShoes_White", 1, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "JoggingShoes_Red", 1, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "JoggingShoes_Blue", 1, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "JoggingShoes_Violet", 1, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "Sneakers_Black", 2, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "Sneakers_Gray", 2, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "Sneakers_White", 2, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "Sneakers_Green", 2, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "Sneakers_Red", 2, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "Wellies_Black", 3, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "Wellies_Grey", 3, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "Wellies_Brown", 3, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "Wellies_Green", 3, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "DressShoes_Black", 8, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "DressShoes_Brown", 8, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "DressShoes_Sunburst", 8, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "DressShoes_Beige", 8, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "DressShoes_White", 8, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "NBCBootsGray", 13, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "NBCBootsYellow", 13, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "WorkingBoots_Brown", 18, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "WorkingBoots_Beige", 18, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "WorkingBoots_Grey", 18, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "WorkingBoots_Green", 18, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "WorkingBoots_Yellow", 18, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "HikingBootsLow_Black", 23, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "HikingBootsLow_Beige", 23, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "HikingBootsLow_Grey", 23, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "HikingBootsLow_Blue", 23, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "JungleBoots_Black", 28, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "JungleBoots_Brown", 28, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "JungleBoots_Beige", 28, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "JungleBoots_Olive", 28, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "JungleBoots_Green", 28, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "HikingBoots_Black", 33, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "HikingBoots_Brown", 33, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "CombatBoots_Black", 38, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "CombatBoots_Brown", 38, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "CombatBoots_Beige", 38, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "CombatBoots_Grey", 38, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "CombatBoots_Green", 38, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "MilitaryBoots_Black", 43, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "MilitaryBoots_Brown", 43, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "MilitaryBoots_Beige", 43, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "MilitaryBoots_Bluerock", 43, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "MilitaryBoots_Redpunk", 43, ""),
            new SpawnConfig("set:dayz_inventory image:feet", "MedievalBoots", 45, "")
        };
    }
    static ref array<ref SpawnConfig> Backpack()
    {
        return {
            new SpawnConfig("set:dayz_inventory image:back", "DrysackBag_Yellow", 5, ""),
            new SpawnConfig("set:dayz_inventory image:back", "DrysackBag_Orange", 5, ""),
            new SpawnConfig("set:dayz_inventory image:back", "DrysackBag_Green", 5, ""),
            new SpawnConfig("set:dayz_inventory image:back", "ChildBag_Red", 10, ""),
            new SpawnConfig("set:dayz_inventory image:back", "ChildBag_Blue", 10, ""),
            new SpawnConfig("set:dayz_inventory image:back", "ChildBag_Green", 10, ""),
            new SpawnConfig("set:dayz_inventory image:back", "SlingBag_Brown", 15, ""),
            new SpawnConfig("set:dayz_inventory image:back", "SlingBag_Gray", 15, ""),
            new SpawnConfig("set:dayz_inventory image:back", "SlingBag_Black", 15, ""),
            new SpawnConfig("set:dayz_inventory image:back", "DuffelBagSmall_Green", 20, ""),
            new SpawnConfig("set:dayz_inventory image:back", "DuffelBagSmall_Camo", 20, ""),
            new SpawnConfig("set:dayz_inventory image:back", "DuffelBagSmall_Medical", 20, ""),
            new SpawnConfig("set:dayz_inventory image:back", "TaloonBag_Orange", 25, ""),
            new SpawnConfig("set:dayz_inventory image:back", "TaloonBag_Blue", 25, ""),
            new SpawnConfig("set:dayz_inventory image:back", "TaloonBag_Violet", 25, ""),
            new SpawnConfig("set:dayz_inventory image:back", "TaloonBag_Green", 25, ""),
            new SpawnConfig("set:dayz_inventory image:back", "AssaultBag_Winter", 30, ""),
            new SpawnConfig("set:dayz_inventory image:back", "AssaultBag_Green", 30, ""),
            new SpawnConfig("set:dayz_inventory image:back", "AssaultBag_Ttsko", 30, ""),
            new SpawnConfig("set:dayz_inventory image:back", "AssaultBag_Black", 30, ""),
            new SpawnConfig("set:dayz_inventory image:back", "DryBag_Yellow", 35, ""),
            new SpawnConfig("set:dayz_inventory image:back", "DryBag_Orange", 35, ""),
            new SpawnConfig("set:dayz_inventory image:back", "DryBag_Red", 35, ""),
            new SpawnConfig("set:dayz_inventory image:back", "DryBag_Blue", 35, ""),
            new SpawnConfig("set:dayz_inventory image:back", "DryBag_Green", 35, ""),
            new SpawnConfig("set:dayz_inventory image:back", "DryBag_Black", 35, ""),
            new SpawnConfig("set:dayz_inventory image:back", "HuntingBag", 40, ""),
            new SpawnConfig("set:dayz_inventory image:back", "TortillaBag_Winter", 45, ""),
            new SpawnConfig("set:dayz_inventory image:back", "TortillaBag", 45, ""),
            new SpawnConfig("set:dayz_inventory image:back", "CoyoteBag_Winter", 50, ""),
            new SpawnConfig("set:dayz_inventory image:back", "CoyoteBag_Brown", 50, ""),
            new SpawnConfig("set:dayz_inventory image:back", "CoyoteBag_Green", 50, ""),
            new SpawnConfig("set:dayz_inventory image:back", "MountainBag_Orange", 55, ""),
            new SpawnConfig("set:dayz_inventory image:back", "MountainBag_Red", 55, ""),
            new SpawnConfig("set:dayz_inventory image:back", "MountainBag_Blue", 55, ""),
            new SpawnConfig("set:dayz_inventory image:back", "MountainBag_Green", 55, ""),
            new SpawnConfig("set:dayz_inventory image:back", "AliceBag_Green", 60, ""),
            new SpawnConfig("set:dayz_inventory image:back", "AliceBag_Camo", 60, ""),
            new SpawnConfig("set:dayz_inventory image:back", "AliceBag_Black", 60, "")
        };
    }
    static ref array<ref SpawnConfig> Hands()
    {
        return {
            new SpawnConfig("set:dayz_inventory image:gloves", "SurgicalGloves_White", 6, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "SurgicalGloves_Green", 6, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "SurgicalGloves_Blue", 6, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "SurgicalGloves_LightBlue", 6, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "WoolGlovesFingerless_Black", 11, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "WoolGlovesFingerless_Green", 11, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "WoolGlovesFingerless_Tan", 11, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "WoolGlovesFingerless_White", 11, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "WoolGlovesFingerless_ChristmasBlue", 11, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "WoolGlovesFingerless_ChristmasRed", 11, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "WoolGloves_Black", 16, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "WoolGloves_Green", 16, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "WoolGloves_Tan", 16, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "WoolGloves_White", 16, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "WoolGloves_ChristmasBlue", 16, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "WoolGloves_ChristmasRed", 16, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "NBCGlovesGray", 21, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "NBCGlovesYellow", 21, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "WorkingGloves_Black", 26, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "WorkingGloves_Brown", 26, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "WorkingGloves_Beige", 26, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "WorkingGloves_Yellow", 26, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "OMNOGloves_Brown", 31, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "OMNOGloves_Gray", 31, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "PaddedGloves_Brown", 36, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "PaddedGloves_Beige", 36, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "PaddedGloves_Threat", 36, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "TacticalGloves_Black", 41, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "TacticalGloves_Green", 41, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "TacticalGloves_Beige", 41, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "SkiGloves_Blue", 46, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "SkiGloves_Red", 46, ""),
            new SpawnConfig("set:dayz_inventory image:gloves", "SkiGloves_90s", 46, "")
        };
    }
    static ref array<ref SpawnConfig> Head()
    {
        return {
            new SpawnConfig("set:dayz_inventory image:headgear", "BaseballCap_Black", 1, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BaseballCap_Beige", 1, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BaseballCap_Olive", 1, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BaseballCap_Camo", 1, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BaseballCap_Blue", 1, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BaseballCap_Pink", 1, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BaseballCap_Red", 1, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "MedicalScrubsHat_White", 2, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "MedicalScrubsHat_Green", 2, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "MedicalScrubsHat_Blue", 2, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "PrisonerCap", 3, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "FlatCap_Black", 4, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "FlatCap_BlackCheck", 4, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "FlatCap_Brown", 4, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "FlatCap_BrownCheck", 4, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "FlatCap_Grey", 4, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "FlatCap_GreyCheck", 4, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "FlatCap_Blue", 4, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "FlatCap_Red", 4, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "SurgicalMask", 5, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "HockeyMask", 6, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "PilotkaCap", 7, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "LeatherHat_Black", 8, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "LeatherHat_Brown", 8, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "LeatherHat_Beige", 8, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "LeatherHat_Natural", 8, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "CowboyHat_black", 10, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "CowboyHat_darkBrown", 10, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "CowboyHat_Brown", 10, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "CowboyHat_green", 10, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "EyeMask_Black", 12, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "EyeMask_Blue", 12, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "EyeMask_Red", 12, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "EyeMask_Yellow", 12, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "EyeMask_Dead", 12, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "EyeMask_Christmas", 12, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "EyeMask_NewYears", 12, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "EyeMask_Valentines", 12, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BoonieHat_Blue", 14, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BoonieHat_Olive", 14, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BoonieHat_Tan", 14, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BoonieHat_Orange", 14, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BoonieHat_Red", 14, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "MimeMask_Male", 16, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "MimeMask_Female", 16, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "PoliceCap", 18, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "OfficerHat", 18, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "NBCHoodGray", 20, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "NBCHoodYellow", 20, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BoonieHat_Black", 22, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BoonieHat_DPM", 22, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BoonieHat_Dubok", 22, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BoonieHat_Flecktran", 22, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "RadarCap_Black", 24, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "RadarCap_Brown", 24, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "RadarCap_Green", 24, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "RadarCap_Blue", 24, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "RadarCap_Red", 24, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "Shemag_White", 25, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "Shemag_Brown", 25, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "Shemag_Green", 25, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "Shemag_Red", 25, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BeanieHat_Black", 27, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BeanieHat_Brown", 27, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BeanieHat_Beige", 27, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BeanieHat_Grey", 27, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BeanieHat_Green", 27, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BeanieHat_Blue", 27, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BeanieHat_Red", 27, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BeanieHat_Pink", 27, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BudenovkaHat_Gray", 28, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "SherpaHat_Black", 29, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "SherpaHat_Blue", 29, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "SherpaHat_Red", 29, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BalaclavaMask_Black", 30, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BalaclavaMask_Green", 30, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BalaclavaMask_Beige", 30, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BalaclavaMask_Blue", 30, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BalaclavaMask_Pink", 30, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BalaclavaMask_White", 30, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "ZmijovkaCap_Black", 32, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "ZmijovkaCap_Brown", 32, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "ZmijovkaCap_Green", 32, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "ZmijovkaCap_Blue", 32, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "ZmijovkaCap_Red", 32, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "Balaclava3Holes_Black", 35, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "Balaclava3Holes_Green", 35, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "Balaclava3Holes_Blue", 35, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "Balaclava3Holes_Beige", 35, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "Balaclava3Holes_White", 35, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "PetushokHat_Black", 37, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "PetushokHat_Green", 37, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "PetushokHat_Yellow", 37, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BalaclavaMask_Pig", 40, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BalaclavaMask_Chain", 40, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BalaclavaMask_Blackskull", 40, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "BalaclavaMask_BDU", 40, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "WitchHat", 42, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "MorozkoHat", 43, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "SantasHat", 44, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "PaydayMask_Chains", 45, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "PaydayMask_Wolf", 45, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "PaydayMask_Hoxton", 45, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "PaydayMask_Dallas", 45, ""),
            new SpawnConfig("set:dayz_inventory image:headgear", "Chainmail_Coif", 50, "")
        };
    }
}

class ItemConfig
{
    string category;  // weapons, ammo/attach, gear, medical
    string classname; // item classname
    int hasrelated;   // 0 = no related, 1 = mag, 2 = ammo, 3 = pouches, 4 = visor, 5 = battery
    // hasrelated 6 = flag to update loose ammo qty in cart (string related will hold the qty count)
    string related;   // related item classname (magazine or ammo)
    int price;        // price for that [x1] item
    int levelRequirement; // required level to buy from safezone

    void ItemConfig(string cat, string clss, int hasrel, string relate, int prc, int Requirement)
    {
        category = cat;
        classname = clss;
        hasrelated = hasrel;
        related = relate;
        price = prc;
        levelRequirement = Requirement;
    }
};

ref array<ref ItemConfig> itemConfigs = 
{
    // Weapons
    new ItemConfig("Weapons", "Izh43Shotgun", 2, "Ammo_12gaPellets", 250, 0),
    new ItemConfig("Weapons", "Mp133Shotgun", 2, "Ammo_12gaPellets", 250, 0),
    new ItemConfig("Weapons", "MP5K", 1, "Mag_MP5_30Rnd", 500, 0),
    new ItemConfig("Weapons", "UMP45", 1, "Mag_UMP_25Rnd", 500, 0),
    new ItemConfig("Weapons", "PP19", 1, "Mag_PP19_64Rnd", 750, 1),
    new ItemConfig("Weapons", "AKS74U", 1, "Mag_AK74_30Rnd", 750, 2),
    new ItemConfig("Weapons", "M16A2", 1, "Mag_STANAG_30Rnd", 750, 4),
    new ItemConfig("Weapons", "AK74", 1, "Mag_AK74_30Rnd", 1000, 6),
    new ItemConfig("Weapons", "Vikhr", 1, "Mag_Vikhr_30Rnd", 1000, 8),
    new ItemConfig("Weapons", "VSS", 1, "Mag_Vikhr_30Rnd", 1250, 10),
    new ItemConfig("Weapons", "ASVAL", 1, "Mag_Vikhr_30Rnd", 1250, 12),
    new ItemConfig("Weapons", "AK101", 1, "Mag_AK101_30Rnd", 1500, 15),
    new ItemConfig("Weapons", "AKM", 1, "Mag_AKM_30Rnd", 1500, 20),
    new ItemConfig("Weapons", "Aug", 1, "Mag_STANAG_30Rnd", 2000, 25),
    new ItemConfig("Weapons", "SawedoffFAMAS", 1, "Mag_FAMAS_25Rnd", 2000, 70),
    new ItemConfig("Weapons", "M4A1", 1, "Mag_STANAG_60Rnd", 2000, 90),

    // Long Weapons
    new ItemConfig("LongWeapons", "SSG82", 1, "Mag_SSG82_5rnd", 500, 3),
    new ItemConfig("LongWeapons", "CZ527", 1, "Mag_CZ527_5rnd", 500, 5),
    new ItemConfig("LongWeapons", "Scout", 1, "Mag_Scout_5Rnd", 500, 7),
    new ItemConfig("LongWeapons", "SKS", 2, "Ammo_762x39", 750, 9),
    new ItemConfig("LongWeapons", "B95", 2, "Ammo_308Win", 750, 11),
    new ItemConfig("LongWeapons", "SV98", 1, "Mag_SV98_10Rnd", 1000, 13),
    new ItemConfig("LongWeapons", "CZ550", 1, "Mag_CZ550_10rnd", 1000, 14),
    new ItemConfig("LongWeapons", "Winchester70", 2, "Ammo_308Win", 1000, 16),
    new ItemConfig("LongWeapons", "Mosin9130", 2, "Ammo_762x54", 1000, 17),
    new ItemConfig("LongWeapons", "SVD", 1, "Mag_SVD_10Rnd", 2000, 80),
    new ItemConfig("LongWeapons", "M14", 1, "Mag_M14_20Rnd", 2000, 100),
    new ItemConfig("LongWeapons", "FAL", 1, "Mag_FAL_20Rnd", 2000, 100),
    
    // Attachments
    new ItemConfig("Attachments", "KobraOptic", 5, "Battery9V", 100, 0),
    new ItemConfig("Attachments", "ReflexOptic", 5, "Battery9V", 100, 0),
    new ItemConfig("Attachments", "PSO1Optic", 5, "Battery9V", 100, 0),
    new ItemConfig("Attachments", "ACOGOptic", 0, "", 100, 0),
    new ItemConfig("Attachments", "HuntingOptic", 0, "", 100, 0),
    new ItemConfig("Attachments", "PUScopeOptic", 0, "", 100, 0),
    new ItemConfig("Attachments", "ACOGOptic_6x", 0, "", 100, 0),
    new ItemConfig("Attachments", "PistolSuppressor", 0, "", 1000, 10),
    new ItemConfig("Attachments", "UniversalSuppressorAK", 0, "", 2000, 60),
    new ItemConfig("Attachments", "UniversalSuppressorM4", 0, "", 2000, 60),

    // Magazines
    new ItemConfig("Magazines", "Mag_IJ70_8Rnd", 2, "Ammo_380", 50, 0),
    new ItemConfig("Magazines", "Mag_P1_8Rnd", 2, "Ammo_9x19", 50, 0),
    new ItemConfig("Magazines", "Mag_1911_7Rnd", 2, "Ammo_45ACP", 50, 0),
    new ItemConfig("Magazines", "Mag_CZ75_15Rnd", 2, "Ammo_9x19", 50, 0),
    new ItemConfig("Magazines", "Mag_Glock_15Rnd", 2, "Ammo_9x19", 50, 0),
    new ItemConfig("Magazines", "Mag_FNX45_15Rnd", 2, "Ammo_45ACP", 50, 0),
    new ItemConfig("Magazines", "Mag_Deagle_9rnd", 2, "Ammo_357", 50, 0),
    new ItemConfig("Magazines", "Mag_CZ61_20Rnd", 2, "Ammo_380", 50, 0),
    new ItemConfig("Magazines", "Mag_MP5_30Rnd", 2, "Ammo_9x19", 50, 0),
    new ItemConfig("Magazines", "Mag_UMP_25Rnd", 2, "Ammo_45ACP", 50, 0),
    new ItemConfig("Magazines", "Mag_PP19_64Rnd", 2, "Ammo_380", 70, 1),
    new ItemConfig("Magazines", "Mag_SSG82_5rnd", 2, "Ammo_545x39", 100, 3),
    new ItemConfig("Magazines", "Mag_AK74_30Rnd", 2, "Ammo_545x39", 70, 2),
    new ItemConfig("Magazines", "Mag_STANAG_30Rnd", 2, "Ammo_556x45", 120, 4),
    new ItemConfig("Magazines", "Mag_CZ527_5rnd", 2, "Ammo_762x39", 120, 5),
    new ItemConfig("Magazines", "Mag_Scout_5Rnd", 2, "Ammo_556x45", 120, 7),
    new ItemConfig("Magazines", "Mag_Vikhr_30Rnd", 2, "Ammo_9x39", 100, 8),
    new ItemConfig("Magazines", "Mag_SV98_10Rnd", 2, "Ammo_762x54", 150, 13),
    new ItemConfig("Magazines", "Mag_CZ550_10rnd", 2, "Ammo_308Win", 150, 14),
    new ItemConfig("Magazines", "Mag_AK101_30Rnd", 2, "Ammo_556x45", 100, 15),
    new ItemConfig("Magazines", "Mag_AKM_30Rnd", 2, "Ammo_762x39", 120, 20),
    new ItemConfig("Magazines", "Mag_AK74_45Rnd", 2, "Ammo_545x39", 150, 30),
    new ItemConfig("Magazines", "Mag_STANAG_60Rnd", 2, "Ammo_556x45", 200, 50),
    new ItemConfig("Magazines", "Mag_AKM_Drum75Rnd", 2, "Ammo_762x39", 200, 50),
    new ItemConfig("Magazines", "Mag_FAMAS_25Rnd", 2, "Ammo_556x45", 150, 70),
    new ItemConfig("Magazines", "Mag_SVD_10Rnd", 2, "Ammo_762x54", 220, 90),
    new ItemConfig("Magazines", "Mag_M14_20Rnd", 2, "Ammo_308Win", 250, 100),
    new ItemConfig("Magazines", "Mag_FAL_20Rnd", 2, "Ammo_308Win", 250, 100),

    // Ammunition
    new ItemConfig("Ammunition", "Ammo_12gaPellets", 6, "10", 20, 0),
    new ItemConfig("Ammunition", "Ammo_12gaSlug", 6, "10", 20, 0),
    new ItemConfig("Ammunition", "Ammo_357", 6, "20", 50, 0),
    new ItemConfig("Ammunition", "Ammo_9x19", 6, "25", 50, 0),
    new ItemConfig("Ammunition", "Ammo_45ACP", 6, "25", 50, 0),
    new ItemConfig("Ammunition", "Ammo_380", 6, "35", 50, 0),
    new ItemConfig("Ammunition", "Ammo_545x39", 6, "20", 70, 0),
    new ItemConfig("Ammunition", "Ammo_9x39", 6, "20", 100, 0), 
    new ItemConfig("Ammunition", "Ammo_556x45", 6, "20", 100, 0), 
    new ItemConfig("Ammunition", "Ammo_762x39", 6, "20", 100, 0),
    new ItemConfig("Ammunition", "Ammo_308Win", 6, "20", 150, 0),
    new ItemConfig("Ammunition", "Ammo_762x54", 6, "20", 150, 0),

    // Grenades
    new ItemConfig("Grenades", "M18SmokeGrenade_White", 0, "", 250, 5),
    new ItemConfig("Grenades", "FlashGrenade", 0, "", 500, 10),
    new ItemConfig("Grenades", "RGD5Grenade", 0, "", 750, 15),
    new ItemConfig("Grenades", "M67Grenade", 0, "", 750, 20),
    new ItemConfig("Grenades", "Grenade_ChemGas", 0, "", 1000, 30),

    // Gear
    new ItemConfig("Gear", "PressVest_Blue", 0, "", 250, 0),
    new ItemConfig("Gear", "Ssh68Helmet", 0, "", 300, 0),
    new ItemConfig("Gear", "BallisticHelmet_Black", 0, "", 500, 5),
    new ItemConfig("Gear", "BallisticHelmet_Green", 0, "", 500, 5),
    new ItemConfig("Gear", "BallisticHelmet_UN", 0, "", 500, 5),
    new ItemConfig("Gear", "PlateCarrierVest_Winter", 3, "PlateCarrierPouches_Winter", 750, 10),
    new ItemConfig("Gear", "PlateCarrierVest_Green", 3, "PlateCarrierPouches_Green", 750, 10),
    new ItemConfig("Gear", "PlateCarrierVest_Black", 3, "PlateCarrierPouches_Black", 750, 10), 
    new ItemConfig("Gear", "Mich2001Helmet", 0, "", 500, 15),
    new ItemConfig("Gear", "GorkaHelmet_Green", 0, "", 500, 20),
    new ItemConfig("Gear", "GorkaHelmet_Black", 0, "", 500, 20),
    new ItemConfig("Gear", "BallisticHelmet_Winter", 0, "", 500, 30),
    new ItemConfig("Gear", "BallisticHelmet_Desert", 0, "", 500, 30),
    new ItemConfig("Gear", "BallisticHelmet_Woodland", 0, "", 500, 30),
    new ItemConfig("Gear", "BallisticHelmet_BDU", 0, "", 500, 30),
    new ItemConfig("Gear", "GhillieSuit_Winter", 0, "", 1000, 40),
    new ItemConfig("Gear", "GhillieSuit_Woodland", 0, "", 1000, 40),
    new ItemConfig("Gear", "GhillieSuit_Mossy", 0, "", 1000, 40),
    new ItemConfig("Gear", "MVS_Combat_Vest_Heavy_OD", 3, "MVS_Heavy_Pouch_OD", 3000, 90),
    new ItemConfig("Gear", "MVS_Combat_Vest_Heavy_Black", 3, "MVS_Heavy_Pouch_Black", 3000, 90),
    new ItemConfig("Gear", "MVS_Altyn_OD", 4, "MVS_Altyn_Visor_OD", 3000, 100),
    new ItemConfig("Gear", "MVS_Altyn_Black", 4, "MVS_Altyn_Visor_Black", 3000, 100),

    // Medical
    new ItemConfig("Medical", "BandageDressing", 0, "", 100, 0),
    new ItemConfig("Medical", "AntiChemInjector", 0, "", 100, 0),
    new ItemConfig("Medical", "TetracyclineAntibiotics", 0, "", 100, 0),
    new ItemConfig("Medical", "SalineBagIV", 0, "", 100, 0),
    new ItemConfig("Medical", "ChelatingTablets", 0, "", 200, 0),
    new ItemConfig("Medical", "Epinephrine", 0, "", 300, 0),
    new ItemConfig("Medical", "BloodBagIV", 0, "", 300, 0),
    new ItemConfig("Medical", "FirstAidKit_BG", 0, "", 300, 0),
    new ItemConfig("Medical", "Morphine", 0, "", 500, 0),

    // Other
    new ItemConfig("Other", "Battery9V", 0, "", 50, 0),
    new ItemConfig("Other", "Heatpack", 0, "", 50, 0),
    new ItemConfig("Other", "Headtorch_Grey", 5, "Battery9V", 100, 0),
    new ItemConfig("Other", "SewingKit", 0, "", 200, 0),
    new ItemConfig("Other", "LeatherSewingKit", 0, "", 300, 0),
    new ItemConfig("Other", "WeaponCleaningKit", 0, "", 300, 0),
    new ItemConfig("Other", "GasMask", 0, "", 500, 0),
    new ItemConfig("Other", "NBCHoodGray", 0, "", 100, 0),
    new ItemConfig("Other", "NBCJacketGray", 0, "", 100, 0),
    new ItemConfig("Other", "NBCPantsGray", 0, "", 100, 0),
    new ItemConfig("Other", "NBCGlovesGray", 0, "", 100, 0),
    new ItemConfig("Other", "NBCBootsGray", 0, "", 100, 0),
};