class TransferMenuUI extends UIHelper
{
    const int ID_CloseSubMenu = 7331;
    const int ID_TransferButton = 1337;
    const int ID_ReturnToMenu = 1357;
    const int ID_Category = 7531;
    const int ID_CopyID = 987;
    const int ID_SendTransfer = 899;
    const int ID_SteamID = 998;
    const int ID_BarterItem = 12345678;

    ref BuyMenuUI m_BuyMenu;
    private Widget m_TransferRoot;
    private Widget m_SendTransferMenu;
    private Widget m_BarterMenu;
    private ButtonWidget m_CloseTransfer;
    private ButtonWidget m_BackButton;
    private TextWidget m_CurrencyAmount;
    private GridSpacerWidget m_CategoryGrid;
    private ButtonWidget m_SellCategoryButton;
    private ButtonWidget m_SendCategoryButton;
    private ButtonWidget m_SendButton;
    private ButtonWidget m_SteamIDButton;
    private TextWidget m_SteamIDText;
    private EditBoxWidget m_SteamIDEditBox;
    private EditBoxWidget m_RecAmountEditBox;
    private ScrollWidget m_BarterScroll;
    private GridSpacerWidget m_BarterGrid;

    private int m_CurrencyAmountInt;
    private string m_SteamID;
    bool m_IsInSafezone = false;

    private ref map<Widget, string> m_WidgetItemList;
    
    int m_Restriction;
    int m_Level;

    void TransferMenuUI() {}
    void ~TransferMenuUI()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(RefreshPlayerInventory);
        CleanupItemPreviews();
        
        PPEffects.SetBlurMenu(0);
        GetGame().GetUIManager().ShowUICursor(false);
        GetGame().GetMission().GetHud().Show(true);
        GetGame().GetMission().PlayerControlEnable(false);
        GetGame().GetInput().ResetGameFocus();
    }

    override Widget Init()
    {
        m_TransferRoot = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/TransferMenuUI.layout");
        m_CloseTransfer = ButtonWidget.Cast(m_TransferRoot.FindAnyWidget("CloseTransferButton"));
        m_BackButton = ButtonWidget.Cast(m_TransferRoot.FindAnyWidget("BackButton"));
        m_CurrencyAmount = TextWidget.Cast(m_TransferRoot.FindAnyWidget("CurrencyAmount"));
        m_CategoryGrid = GridSpacerWidget.Cast(m_TransferRoot.FindAnyWidget("CategoryGrid"));

        m_SellCategoryButton = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/CategoryButton2.layout", m_CategoryGrid));
        TextWidget.Cast(m_SellCategoryButton.FindAnyWidget("CategoryText")).SetText("SELL");

        m_SendCategoryButton = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/CategoryButton2.layout", m_CategoryGrid));
        TextWidget.Cast(m_SendCategoryButton.FindAnyWidget("CategoryText")).SetText("SEND");

        m_WidgetItemList = new map<Widget, string>;
        SetCategory(0);

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player) 
        { 
            m_SteamID = player.GetIdentity().GetPlainId(); 
        }

        PPEffects.SetBlurMenu(0.3);
        GetGame().GetUIManager().ShowUICursor(true);
        GetGame().GetMission().GetHud().Show(false);
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
        GetGame().GetInput().ChangeGameFocus(1);
        SetID(MENU_TRANSFER); 

        return m_TransferRoot;
    }

    void ShowBarterMenu()
    {
        if (!m_BarterMenu)
        {
            m_BarterMenu = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/BarterMenu.layout", m_TransferRoot);
            m_BarterScroll = ScrollWidget.Cast(m_BarterMenu.FindAnyWidget("ScrollWidget1"));
            m_BarterGrid = GridSpacerWidget.Cast(m_BarterMenu.FindAnyWidget("GridSpacerWidget1"));
            m_BarterScroll.VScrollToPos01(0);
        }

        PopulateBarterItems();
    }

    void PopulateBarterItems()
    {
        ClearItemsFromMenu();

        array<ref BarterInfo> barterItems = BarterConfig.GetAllBarterItems();
        if (!barterItems || barterItems.Count() == 0)
            return;

        foreach (BarterInfo barterItem : barterItems)
        {
            Widget itemWidget = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/BuyMenuItemUI.layout", m_BarterGrid);
            TextWidget itemTextWidget = TextWidget.Cast(itemWidget.FindAnyWidget("ItemText"));
            TextWidget priceTextWidget = TextWidget.Cast(itemWidget.FindAnyWidget("PriceText"));
            Widget blackMarketWidget = Widget.Cast(itemWidget.FindAnyWidget("BlackMarket"));

            blackMarketWidget.Show(true);
            string displayName = ItemHelper.GetDisplayName(barterItem.classname);
            itemTextWidget.SetText(displayName);
            priceTextWidget.SetText("$" + FormatCurrency(barterItem.worth));

            AddItemPreview(barterItem.classname, itemWidget);
            m_WidgetItemList.Insert(itemWidget, barterItem.classname);
        }

        RefreshPlayerInventory();
    }

    void ClearItemsFromMenu()
    {
        Widget child = m_BarterGrid.GetChildren();
        while (child)
        {
            Widget next = child.GetSibling();
            child.Unlink();
            child = next;
        }
        m_WidgetItemList.Clear();
    }

    void RefreshPlayerInventory()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        foreach (Widget widget, string classname : m_WidgetItemList)
        {
            Widget blackMarketWidget = Widget.Cast(widget.FindAnyWidget("BlackMarket"));
            if (!blackMarketWidget)
                continue;

            if (player.HasItemInInventory(classname))
                blackMarketWidget.Show(false);
            else blackMarketWidget.Show(true);
        }
    }

    void ShowTransferMenu()
    {
        if (!m_SendTransferMenu)
        {
            m_SendTransferMenu = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/SendTransferMenu.layout", m_TransferRoot);
            m_SteamIDButton = ButtonWidget.Cast(m_SendTransferMenu.FindAnyWidget("SteamIDButton"));
            m_SteamIDText = TextWidget.Cast(m_SendTransferMenu.FindAnyWidget("SteamIDText"));
            m_SteamIDEditBox = EditBoxWidget.Cast(m_SendTransferMenu.FindAnyWidget("SteamIDEditBox"));
            m_RecAmountEditBox = EditBoxWidget.Cast(m_SendTransferMenu.FindAnyWidget("RecAmountEditBox"));
            m_SendButton = ButtonWidget.Cast(m_SendTransferMenu.FindAnyWidget("SendButton"));
        }
    }

    void ProcessSend()
    {
        if (m_SteamIDEditBox && m_RecAmountEditBox)
        {
            string recipientSteamID = m_SteamIDEditBox.GetText();
            string amountText = m_RecAmountEditBox.GetText();
            if (IsNumber(recipientSteamID) && recipientSteamID.Length() == 17 && recipientSteamID != m_SteamID)
            {
                if (IsNumber(amountText))
                {
                    int transferAmount = amountText.ToInt();
                    if (transferAmount <= m_CurrencyAmountInt && transferAmount != 0)
                    {
                        auto params = new Param2<string, int>(recipientSteamID, transferAmount);
                        GetRPCManager().SendRPC("TransferMenu", "TransferFunds", params);
                        m_SteamIDEditBox.SetText("");
                        m_RecAmountEditBox.SetText("");
                    }
                    else
                    {
                        NotificationSystem.AddNotificationExtended(NOTIFY_LENGTH, NOTIFY_TRANSFER_FAILED, NOTIFY_TRANSFER_FAILED_INSUFFICIENT);
                    }
                }
                else
                {
                    NotificationSystem.AddNotificationExtended(NOTIFY_LENGTH, NOTIFY_TRANSFER_FAILED, NOTIFY_TRANSFER_FAILED_INVALID);
                }
            }
            else
            {
                NotificationSystem.AddNotificationExtended(NOTIFY_LENGTH, NOTIFY_TRANSFER_FAILED, NOTIFY_TRANSFER_FAILED_INVALID);
            }
        }
        m_SteamIDEditBox.SetText("");
        m_RecAmountEditBox.SetText("");
    }

        void OnBarterItem(Widget w)
    {
        Widget blackMarketWidget = Widget.Cast(w.FindAnyWidget("BlackMarket"));
        blackMarketWidget.Show(true);
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(RefreshPlayerInventory, 300, false);
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w.GetUserID() == ID_BarterItem)
        {
            Widget currentWidget = w;
            while (currentWidget && !m_WidgetItemList.Contains(currentWidget))
            {
                currentWidget = currentWidget.GetParent();
            }

            if (currentWidget)
            {
                string classname = m_WidgetItemList.Get(currentWidget);
                GetRPCManager().SendRPC("TransferMenu", "BarterTransaction", new Param1<string>(classname), true);
                OnBarterItem(currentWidget);
            }

            return true;
        }

        if (w == m_SellCategoryButton)
        {
            SetCategory(0);
            return true;
        }

        if (w == m_SendCategoryButton)
        {
            SetCategory(1);
            return true;
        }

        if (w.GetUserID() == ID_CopyID)
        {
            CopySteamIDToClipboard();
            return true;
        }

        if (w.GetUserID() == ID_SendTransfer)
        {
            ProcessSend();
            return true;
        }

        if (w.GetUserID() == ID_SteamID)
        {
            if (m_SteamIDButton.GetState())
            {
                m_SteamIDText.SetText(m_SteamID);
            }
            else
            {
                m_SteamIDText.SetText("[SHOW STEAM ID]");
            }
            return true;
        }

        if (w.GetUserID() == ID_ReturnToMenu)
        {
            Close();

            UIManager UIMgr = GetGame().GetUIManager();
            if (UIMgr)
            {
                m_BuyMenu = BuyMenuUI.Cast(UIMgr.ShowScriptedMenu(new BuyMenuUI(), null));
                m_BuyMenu.SetCurrency(m_CurrencyAmountInt);
                m_BuyMenu.SetBuyRestrictions(m_Restriction, m_Level);
            }
            return true;
        }

        if (w.GetUserID() == ID_CloseSubMenu)
        {
            Close();
            return true;
        }

        return false;
    }

    void SetBuyRestrictions(int restrict, int playerLevel)
    {
        m_Restriction = restrict;
        m_Level = playerLevel;
    }

    void SetCurrency(int newCurrency)
    {
        m_CurrencyAmountInt = newCurrency;
        if (m_CurrencyAmount)
        {
            m_CurrencyAmount.SetText("$" + FormatCurrency(newCurrency));
        }
    }

    void CopySteamIDToClipboard()
    {
        if (m_SteamID && m_SteamID.Length() > 0)
        { GetGame().CopyToClipboard(m_SteamID); }
    }

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
            HideTransferMenu();
            m_SellCategoryButton.SetState(true);
            ShowBarterMenu();
        }
        else if (categoryIndex == 1)
        {
            HideBarterMenu();
            m_SendCategoryButton.SetState(true);
            ShowTransferMenu();
        }
    }

    void HideBarterMenu()
    {
        if (m_BarterMenu)
        { 
            m_BarterMenu.Unlink();
            m_BarterMenu = null;
        }
    }

    void HideTransferMenu()
    {
        if (m_SendTransferMenu)
        { 
            m_SendTransferMenu.Unlink();
            m_SendTransferMenu = null;
        }
    }

    bool IsNumber(string input)
    {
        for (int i = 0; i < input.Length(); i++)
        {
            int charAscii = input.Substring(i, 1).ToAscii();
            if (charAscii < 48 || charAscii > 57)
            { return false; }
        }
        return true;
    }

    bool IsFocusable(Widget w)
    {
        if (w && w.IsInherited(EditBoxWidget))
            return true;

        if (w.GetUserID() == ID_CloseSubMenu || w.GetUserID() == ID_TransferButton || w.GetUserID() == ID_ReturnToMenu || w.GetUserID() == ID_Category || w.GetUserID() == ID_CopyID || w.GetUserID() == ID_SendTransfer || w.GetUserID() == ID_SteamID || w.GetUserID() == ID_BarterItem)
            return true;

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