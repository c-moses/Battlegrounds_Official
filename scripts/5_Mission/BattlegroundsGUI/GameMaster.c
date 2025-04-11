class GameMasterUI extends UIHelper
{
    private ScrollWidget                m_ScrollWidget;
    private GridSpacerWidget            m_GridSpacerWidget;
    private ref map<Widget, string>     m_LayersMap;

    private EditBoxWidget               m_RoundTime;
    private EditBoxWidget               m_ScoreLimit;
    private EditBoxWidget               m_BombTime;
    private EditBoxWidget               m_PreRoundTime;
    private EditBoxWidget               m_RoundSwitch;

    private string m_SelectedLayer = "";

    void ~GameMasterUI()
    {
        PPEffects.SetBlurMenu(0);
        GetGame().GetUIManager().ShowUICursor(false);
        GetGame().GetMission().GetHud().Show(true);
        GetGame().GetMission().PlayerControlEnable(false);
        GetGame().GetInput().ResetGameFocus();
    }

    override Widget Init()
    {
        layoutRoot                      = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/GameMaster.layout");
        m_ScrollWidget                  = ScrollWidget.Cast(layoutRoot.FindAnyWidget("ScrollWidget"));
        m_GridSpacerWidget              = GridSpacerWidget.Cast(m_ScrollWidget.FindAnyWidget("GridSpacerWidget"));

        m_RoundTime                     = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("EditBoxRoundTime"));
        m_ScoreLimit                    = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("EditBoxScoreLimit"));
        m_BombTime                      = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("EditBoxBombTime"));
        m_PreRoundTime                  = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("EditBoxPreRoundTime"));
        m_RoundSwitch                   = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("EditBoxRoundSwitch"));

        m_LayersMap = new map<Widget, string>;
        m_ScrollWidget.VScrollToPos01(0);

        PPEffects.SetBlurMenu(0.3);
        GetGame().GetUIManager().ShowUICursor(true);
        GetGame().GetMission().GetHud().Show(false);
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
        GetGame().GetInput().ChangeGameFocus(1);
        SetID(MENU_GAMEMASTER);
        SetLayers();

        return layoutRoot;
    }

    void SetLayers()
    {
        ClearLayers();
        m_LayersMap.Clear();
        m_ScrollWidget.VScrollToPos01(0);

        ref array<ref LayerInfo> allLayers = new array<ref LayerInfo>();
        ref array<ref LayerInfo> chernarusLayers = MapLayerConfig.GetChernarusplusLayers();
        if (chernarusLayers)
        {
            foreach (LayerInfo chernaruslayer : chernarusLayers)
            {
                allLayers.Insert(chernaruslayer);
            }
        }

        ref array<ref LayerInfo> enochLayers = MapLayerConfig.GetEnochLayers();
        if (enochLayers)
        {
            foreach (LayerInfo enochlayer : enochLayers)
            {
                allLayers.Insert(enochlayer);
            }
        }

        ref array<ref LayerInfo> sakhalLayers = MapLayerConfig.GetSakhalLayers();
        if (sakhalLayers)
        {
            foreach (LayerInfo sakhallayer : sakhalLayers)
            {
                allLayers.Insert(sakhallayer);
            }
        }

        foreach (LayerInfo layer : allLayers)
        {
            AddLayerButton(layer);
        }

        m_GridSpacerWidget.Update();
    }

    void AddLayerButton(LayerInfo layer)
    {
        ButtonWidget layerEntry = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/GameMasterEntry.layout", m_GridSpacerWidget));
        TextWidget entryWidget = TextWidget.Cast(layerEntry.FindAnyWidget("LayerEntryText"));
        entryWidget.SetText(layer.layerName);
        m_LayersMap.Insert(layerEntry, layer.layerCode);
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (m_LayersMap.Contains(w))
        {
            ButtonWidget layerButton = ButtonWidget.Cast(w);
            if (m_SelectedLayer == m_LayersMap.Get(w))
            {
                layerButton.SetState(false);
                m_SelectedLayer = "";
            }
            else
            {
                ClearButtonState();
                layerButton.SetState(true);
                m_SelectedLayer = m_LayersMap.Get(w);
            }

            return true;
        }

        if (w.GetUserID() == 69)
        {
            if (m_SelectedLayer != "")
            {
                float roundTime = m_RoundTime.GetText().ToFloat();
                int scoreLimit = m_ScoreLimit.GetText().ToInt();
                float bombTime = m_BombTime.GetText().ToFloat();
                float preRoundTime = m_PreRoundTime.GetText().ToFloat();
                int roundSwitch = m_RoundSwitch.GetText().ToInt();
                GetRPCManager().SendRPC("Battlegrounds", "GameMaster", new Param6<string, float, int, float, float, int>(m_SelectedLayer, roundTime, scoreLimit, bombTime, preRoundTime, roundSwitch));
                Close();
            }
            return true;
        }
        if (w.GetUserID() == 99)
        {
            Close();
            return true;
        }
        return false;
    }

    void ClearButtonState()
    {
        Widget child = m_GridSpacerWidget.GetChildren();
        while (child)
        {
            ButtonWidget layerButton = ButtonWidget.Cast(child);
            layerButton.SetState(false);
            child = child.GetSibling();
        }
    }

    void ClearLayers()
    {
        Widget child = m_GridSpacerWidget.GetChildren();
        while (child)
        {
            Widget next = child.GetSibling();
            child.Unlink();
            child = next;
        }
    }

    bool IsFocusable(Widget w)
    {
        if (w.GetUserID() == 69 || w.GetUserID() == 99 || w.GetUserID() == 420)
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