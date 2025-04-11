modded class RespawnDialogue extends UIScriptedMenu
{
    // IDC_CANCEL is ID2
    const int 			        ID_RESPAWN = 1;
    const int                   ID_CATEGORY = 901;
    const int                   ID_EQUIP = 12345678;

    protected ButtonWidget          m_RespawnButton;
    protected XComboBoxWidget       m_CharacterSelect;
    protected PlayerPreviewWidget   m_PreviewWidget;
    protected PlayerBase            m_PreviewCharacter;
    protected EntityAI              m_PreviewSidearm;
    protected GridSpacerWidget      m_GridSpacerWidgetCat;
    protected GridSpacerWidget      m_CategoryGridSpacer;
    protected ScrollWidget          m_ScrollWidget;

    protected int                   m_CurrentCategory;
    protected int                   m_CurrentCharacter;
    protected ref array<int>        m_CurrentLoadout;

    protected ref array<string>         m_UniqueCategories;
    protected ref map<Widget, int>      m_CategoryButtonsMap;
    protected ref map<int, Widget>      m_WidgetItemList;   

    void RespawnDialogue();
	void ~RespawnDialogue()
    {
        if (m_PreviewCharacter)
        {
            GetGame().ObjectDelete(m_PreviewCharacter);
            m_PreviewCharacter = null;
        }
    }

    override Widget Init()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player)
            player.ShowDeadScreen(false, 0); // hack

        layoutRoot = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/RespawnDialogue.layout");
        m_RespawnButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("RespawnButton"));
        m_CharacterSelect = XComboBoxWidget.Cast(layoutRoot.FindAnyWidget("XComboBoxWidget"));
        m_PreviewWidget = PlayerPreviewWidget.Cast(layoutRoot.FindAnyWidget("PlayerPreviewWidget"));
        m_GridSpacerWidgetCat = GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("GridSpacerWidget")); // holds categories
        m_CategoryGridSpacer = GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("CategoryGridSpacer")); // holds items
        m_ScrollWidget = ScrollWidget.Cast(layoutRoot.FindAnyWidget("CategoryScroll"));

        MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
        if (mission)
        {
            m_CurrentLoadout = new array<int>();
            m_CurrentLoadout.Copy(mission.GetSavedLoadout());

            if (m_CurrentLoadout.Count() > 0)
                m_CurrentCharacter = m_CurrentLoadout[0];

            else m_CurrentCharacter = 0;
        }

        m_UniqueCategories = new array<string>;
        m_CategoryButtonsMap = new map<Widget, int>;
        m_WidgetItemList = new map<int, Widget>;

        PopulateCharacters();
        PopulateCategories();
        SetCategory(0);
        UpdatePlayerPreview();

        return layoutRoot;
    }

    void PopulateCharacters()
    {
        ref array<ref SpawnConfig> characters = SpawnMenuItems.Character();
        if (!characters)
            return;

        foreach (SpawnConfig config : characters)
        {
            string displayName = GetCharacterName(config.classname);
            m_CharacterSelect.AddItem(displayName);
        }

        m_CharacterSelect.SetCurrentItem(m_CurrentCharacter);
        m_CharacterSelect.Update();
    }

    void PopulateCategories()
    {
        Widget child = m_GridSpacerWidgetCat.GetChildren();
        while (child)
        {
            Widget next = child.GetSibling();
            child.Unlink();
            child = next;
        }

        m_UniqueCategories.Clear();
        m_CategoryButtonsMap.Clear();

        array<string> categoryImages = GetCategoryImages();
        for (int i = 0; i < categoryImages.Count(); i++)
        {
            string categoryImage = categoryImages[i];
            m_UniqueCategories.Insert(categoryImage);

            ButtonWidget categoryButton = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/CategoryButton.layout", m_GridSpacerWidgetCat));
            ImageWidget categoryImageWidget = ImageWidget.Cast(categoryButton.FindAnyWidget("CategoryImage"));

            if (categoryImageWidget)
            {
                categoryImageWidget.LoadImageFile(0, categoryImage);
            }

            m_CategoryButtonsMap.Insert(categoryButton, i);
        }
    }

    void SetCategory(int categoryIndex)
    {
        Widget child = m_GridSpacerWidgetCat.GetChildren();
        while (child)
        {
            ButtonWidget categoryButton = ButtonWidget.Cast(child);
            categoryButton.SetState(false);
            child = child.GetSibling();
        }

        if (categoryIndex >= 0 && categoryIndex < m_UniqueCategories.Count())
        {
            m_CurrentCategory = categoryIndex;

            ButtonWidget selectedButton = ButtonWidget.Cast(m_GridSpacerWidgetCat.GetChildren());
            for (int i = 0; i < categoryIndex; i++)
            {
                selectedButton = ButtonWidget.Cast(selectedButton.GetSibling());
            }
            if (selectedButton)
            {
                selectedButton.SetState(true);
            }

            m_WidgetItemList.Clear();
            PopulateItemsForCategory(categoryIndex);
        }
    }

    void PopulateItemsForCategory(int selectedCategoryIndex)
    {
        ClearItemsFromMenu();
        m_ScrollWidget.VScrollToPos01(0);

        string selectedCategory = m_UniqueCategories[selectedCategoryIndex];
        MissionGameplay missionGameplay = MissionGameplay.Cast(GetGame().GetMission());
        int playerLevel = missionGameplay.m_StatLevel;

        int favouriteIndex = -1;
        if (selectedCategoryIndex >= 0 && selectedCategoryIndex + 1 < m_CurrentLoadout.Count())
        {
            favouriteIndex = m_CurrentLoadout[selectedCategoryIndex + 1];
        }

        ref array<ref SpawnConfig> categoryItems;
        switch (selectedCategory)
        {
            case "set:dayz_inventory image:pistol": categoryItems = SpawnMenuItems.Sidearm(); break;
            case "set:dayz_inventory image:body": categoryItems = SpawnMenuItems.Torso(); break;
            case "set:dayz_inventory image:legs": categoryItems = SpawnMenuItems.Legs(); break;
            case "set:dayz_inventory image:feet": categoryItems = SpawnMenuItems.Feet(); break;
            case "set:dayz_inventory image:back": categoryItems = SpawnMenuItems.Backpack(); break;
            case "set:dayz_inventory image:gloves": categoryItems = SpawnMenuItems.Hands(); break;
            case "set:dayz_inventory image:headgear": categoryItems = SpawnMenuItems.Head(); break;
            default: return;
        }
        
        int itemIndex = 0;
        foreach (SpawnConfig config : categoryItems)
        {
            Widget itemWidget = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/BuyMenuItemUI.layout", m_CategoryGridSpacer);
            ButtonWidget itemButton = ButtonWidget.Cast(itemWidget.FindAnyWidget("ItemButton"));
            TextWidget itemTextWidget = TextWidget.Cast(itemWidget.FindAnyWidget("ItemText"));
            TextWidget levelTextWidget = TextWidget.Cast(itemWidget.FindAnyWidget("PriceText"));
            Widget blackMarketWidget = Widget.Cast(itemWidget.FindAnyWidget("BlackMarket"));

            string displayName = ItemHelper.GetDisplayName(config.classname);
            itemTextWidget.SetText(displayName);
            levelTextWidget.SetText("LEVEL " + config.levelRequirement);

            if (playerLevel < config.levelRequirement)
            {
                blackMarketWidget.Show(true);
                levelTextWidget.SetColor(ARGB(255, 190, 95, 95));
            }

            if (itemIndex == favouriteIndex)
            {
                itemButton.SetState(true);
            }

            AddItemPreview(config.classname, itemWidget);

            m_WidgetItemList.Insert(itemIndex, itemButton);
            itemIndex++;
        }
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        switch (w.GetUserID())
        {
            case IDC_CANCEL:
                Close();
                PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
                if (player) 
                { 
                    player.ShowDeadScreen(true, 0); 
                }
                return true;

            case ID_RESPAWN:
                MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
                if (mission)
                    mission.SetSavedLoadout(m_CurrentLoadout);

                string formattedLoadout = FormatLoadoutData();
                string formattedHotkeys = FormatHotkeyData();
                GetRPCManager().SendRPC("LoadoutSelect", "RequestLoadout", new Param2<string, string>(formattedLoadout, formattedHotkeys));
                return RequestRespawn(true);
                
            case ID_EQUIP:
                foreach (int itemIndex, Widget itemWidget : m_WidgetItemList)
                {
                    if (w == itemWidget || w.GetParent() == itemWidget)
                    {
                        EquipItem(itemIndex, w);
                        UpdatePlayerPreview();
                        return true;
                    }
                }
                break;

            break;
        }

        if (m_CategoryButtonsMap.Contains(w))
        {
            int categoryIndex = m_CategoryButtonsMap.Get(w);
            SetCategory(categoryIndex);
            return true;
        }

        return false;
    }

    override bool OnChange(Widget w, int x, int y, bool finished)
    {
        if (w == m_CharacterSelect)
        {
            int selectedIndex = m_CharacterSelect.GetCurrentItem();
            if (selectedIndex >= 0)
            {
                m_CurrentCharacter = selectedIndex;
                if (m_CurrentLoadout && m_CurrentLoadout.Count() > 0)
                    m_CurrentLoadout[0] = m_CurrentCharacter;

                ref array<ref SpawnConfig> characters = SpawnMenuItems.Character();
                if (characters && selectedIndex < characters.Count())
                {
                    m_CharacterSelect.SetCurrentItem(m_CurrentCharacter);
                    m_CharacterSelect.Update();

                    GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(UpdatePlayerPreview, 20, false);
                }
                return true;
            }
        }
        return false;
    }

    void EquipItem(int itemIndex, Widget clickedWidget)
    {
        int loadoutIndex = m_CurrentCategory + 1;
        if (loadoutIndex < m_CurrentLoadout.Count())
        {
            ButtonWidget clickedButton = ButtonWidget.Cast(clickedWidget);
            if (m_CurrentLoadout[loadoutIndex] == itemIndex)
            {
                if (clickedButton)
                    clickedButton.SetState(false);

                m_CurrentLoadout[loadoutIndex] = -1;
                return;
            }

            foreach (int otherItemIndex, Widget otherWidget : m_WidgetItemList)
            {
                if (otherWidget)
                {
                    ButtonWidget otherButton = ButtonWidget.Cast(otherWidget);
                    if (otherButton)
                        otherButton.SetState(false);
                }
            }

            if (clickedButton)
                clickedButton.SetState(true);

            m_CurrentLoadout[loadoutIndex] = itemIndex;
        }
    }

    void AddItemPreview(string itemType, Widget itemWidget)
    {
        ItemPreviewWidget previewWidget = ItemPreviewWidget.Cast(itemWidget.FindAnyWidget("ItemPreview"));
        if (!previewWidget) return;

        EntityAI previewItem = EntityAI.Cast(GetGame().CreateObject(itemType, "0 0 0", true, false, true));
        if (!previewItem) return;

        previewWidget.SetItem(previewItem);
        previewWidget.SetModelPosition(Vector(0, 0, 0.5));
        previewWidget.SetModelOrientation(vector.Zero);
        previewWidget.SetView(previewItem.GetViewIndex());
    }

    void UpdatePlayerPreview()
    {
        ClearCharacterPreview();

        string characterClassname = GetClassnameByLoadoutIndex(0);
        string sidearmClassname = GetClassnameByLoadoutIndex(1);
        AddCharacterPreview(characterClassname, sidearmClassname);

        for (int loadoutIndex = 2; loadoutIndex < m_CurrentLoadout.Count(); loadoutIndex++)
        {
            string classname = GetClassnameByLoadoutIndex(loadoutIndex);
            if (classname != "")
            {
                m_PreviewCharacter.GetInventory().CreateInInventory(classname);
            }
        }

        m_PreviewWidget.Refresh();
    }

    void ClearCharacterPreview()
    {
        if (m_PreviewCharacter)
        {
            GetGame().ObjectDelete(m_PreviewCharacter);
            m_PreviewCharacter = null;
        }

        if (m_PreviewSidearm)
        {
            GetGame().ObjectDelete(m_PreviewSidearm);
            m_PreviewSidearm = null;
        }
    }

    void AddCharacterPreview(string classname, string sidearmClassname)
    {
        vector cameraDirection = GetGame().GetCurrentCameraDirection();
        cameraDirection.Normalize();
        vector adjustedPosition = GetGame().GetCurrentCameraPosition() - cameraDirection * 0.5;
        adjustedPosition[1] = GetGame().GetPlayer().GetPosition()[1];

        m_PreviewCharacter = PlayerBase.Cast(GetGame().CreateObjectEx(classname, adjustedPosition, ECE_LOCAL | ECE_NOLIFETIME));

        if (m_PreviewCharacter)
        {
            m_PreviewWidget.SetPlayer(m_PreviewCharacter);

            if (sidearmClassname != "")
            {
                EntityAI sidearm = EntityAI.Cast(GetGame().CreateObjectEx(sidearmClassname, vector.Zero, ECE_LOCAL | ECE_NOLIFETIME));
                if (sidearm)
                {
                    m_PreviewSidearm = sidearm;
                    m_PreviewWidget.UpdateItemInHands(m_PreviewSidearm);
                }
            }

            m_PreviewWidget.SetModelPosition(Vector(0, 0, 0.5));
            m_PreviewWidget.SetModelOrientation(Vector(0, 0, 0));
            m_PreviewWidget.SetSize(1, 1);
        }
    }

    // UTILITY

    string GetCharacterName(string classname)
    {
        if (classname.Contains("SurvivorM_"))
        {
            return classname.Substring(10, classname.Length() - 10);
        }
        else if (classname.Contains("SurvivorF_"))
        {
            return classname.Substring(10, classname.Length() - 10);
        }
        return "";
    }

    string GetClassnameByLoadoutIndex(int loadoutIndex)
    {
        if (!m_CurrentLoadout || loadoutIndex < 0 || loadoutIndex >= m_CurrentLoadout.Count())
            return "";

        int itemIndex = m_CurrentLoadout[loadoutIndex];
        if (itemIndex < 0)
            return "";

        ref array<ref SpawnConfig> items;
        switch (loadoutIndex)
        {
            case 0: items = SpawnMenuItems.Character(); break; // Character
            case 1: items = SpawnMenuItems.Sidearm(); break;   // Sidearm
            case 2: items = SpawnMenuItems.Torso(); break;     // Torso
            case 3: items = SpawnMenuItems.Legs(); break;      // Legs
            case 4: items = SpawnMenuItems.Feet(); break;      // Feet
            case 5: items = SpawnMenuItems.Backpack(); break;  // Backpack
            case 6: items = SpawnMenuItems.Hands(); break;     // Hands
            case 7: items = SpawnMenuItems.Head(); break;      // Head
            default: return "";
        }

        if (items && itemIndex < items.Count())
            return items.Get(itemIndex).classname;

        return "";
    }

    array<string> GetCategoryImages()
    {
        return {
            "set:dayz_inventory image:pistol",   // Sidearm
            "set:dayz_inventory image:body",     // Torso
            "set:dayz_inventory image:legs",     // Legs
            "set:dayz_inventory image:feet",     // Feet
            "set:dayz_inventory image:back",     // Backpack
            "set:dayz_inventory image:gloves",   // Hands
            "set:dayz_inventory image:headgear"  // Head
        };
    }

    string FormatLoadoutData()
    {
        string loadout;
        for (int categoryIndex = 0; categoryIndex < m_CurrentLoadout.Count(); categoryIndex++)
        {
            int itemIndex = m_CurrentLoadout[categoryIndex];
            loadout += categoryIndex.ToString() + ";" + itemIndex.ToString() + " ";
        }

        loadout = loadout.Trim();
        return loadout;
    }

    string FormatHotkeyData()
    {
        TStringArray actionNames = {"UASidearmHotkey", "UABandageHotkey", "UAKnifeHotkey"};
        TStringArray hotkeys = new TStringArray();

        foreach (string actionName : actionNames)
        {
            string keyName = InputUtils.GetButtonNameFromInput(actionName, EUAINPUT_DEVICE_KEYBOARD);
            if (keyName.Length() == 1 && keyName.ToInt() >= 1 && keyName.ToInt() <= 9)
            {
                hotkeys.Insert(keyName);
            }
            else
            {
                hotkeys.Insert("-1");
            }
        }

        string hotkeyString = "";
        for (int i = 0; i < hotkeys.Count(); i++)
        {
            hotkeyString += hotkeys[i];
            if (i < hotkeys.Count() - 1)
            {
                hotkeyString += ",";
            }
        }

        return hotkeyString;
    }

    void ClearItemsFromMenu()
    {
        Widget child = m_CategoryGridSpacer.GetChildren();
        while (child)
        {
            Widget next = child.GetSibling();
            child.Unlink();
            child = next;
        }
    }

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		ColorHighlight(w);
		return false;
	}

	override bool OnFocus(Widget w, int x, int y)
	{
		if (IsFocusable(w))
		{
			ColorHighlight(w);
			return true;
		}

		return false;
	}

    override bool IsFocusable(Widget w)
    {
        if (w.GetUserID() == ID_RESPAWN || w.GetUserID() == ID_CATEGORY || w.GetUserID() == ID_EQUIP || w.GetUserID() == IDC_CANCEL)
            return true;

        if (w == m_CharacterSelect)
            return true;

        return false;
    }

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		ColorNormal(w);
		return true;
	}

    override void ColorNormal(Widget w)
    {
        if (!w)
			return;

        SetFocus(null);
    }
}