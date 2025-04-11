class CurrencyUI extends UIHelper
{
    private TextWidget m_CurrencyText;
    private int m_CurrencyAmount;

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/CurrencyUI.layout");
        m_CurrencyText = TextWidget.Cast(layoutRoot.FindAnyWidget("CurrencyText"));
        SetID(UI_CURRENCY);
        return layoutRoot;
    }

    void UpdateCurrencyDisplay(int amount)
    {
        m_CurrencyAmount = amount;
        string formattedCurrency = FormatCurrency(amount);
        m_CurrencyText.SetText("$" + formattedCurrency);
    }
}