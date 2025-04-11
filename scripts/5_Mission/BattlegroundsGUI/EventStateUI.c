class EventStateUI extends UIHelper
{
    private TextWidget m_Timer;
    private float m_EndTime;
    private bool m_IsTimerActive = false;
    private TextWidget m_EventState; // show(false) on init
    private ImageWidget m_KeyImage; // show(false) on init
    private Widget m_KeyText; // show(false) on init
    private TextWidget m_KeyTextShort; // show(true) on init, but a child of m_KeyImage
    private TextWidget m_KeyTextLong; // show(true) on init, but a child of m_KeyText

    void ~EventStateUI()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.TimerUpdate);
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CheckSZStatus);
    }

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/EventTimerUI.layout");
        m_Timer = TextWidget.Cast(layoutRoot.FindAnyWidget("Timer"));
        m_EventState = TextWidget.Cast(layoutRoot.FindAnyWidget("EventState"));
        m_KeyImage = ImageWidget.Cast(layoutRoot.FindAnyWidget("KeyImage"));
        m_KeyText = Widget.Cast(layoutRoot.FindAnyWidget("KeyText"));
        m_KeyTextShort = TextWidget.Cast(layoutRoot.FindAnyWidget("KeyTextShort"));
        m_KeyTextLong = TextWidget.Cast(layoutRoot.FindAnyWidget("KeyTextLong"));
        SetID(UI_EVENTTIMER);
        return layoutRoot;
    }

// =============== STATE ===============

    void ShowState(bool show, int state, int round = -1)
    {
        if (show)
        {
            m_EventState.Show(true);
            if (state == BZ_STATE_TRUE || state == SND_STATE_ACTIVE_BUY)
            {
                m_EventState.SetText("BUY MENU ");
                m_EventState.SetColor(ARGB(255, 40, 160, 70));
                string bzKeyName = InputUtils.GetButtonNameFromInput("UABuyMenuToggle", EUAINPUT_DEVICE_KEYBOARD);
                if (bzKeyName.Length() <= 2)
                {
                    m_KeyText.Show(false);
                    m_KeyImage.Show(true);
                    m_KeyTextShort.SetText(bzKeyName);
                }
                else
                {
                    m_KeyImage.Show(false);
                    m_KeyText.Show(true);
                    m_KeyTextLong.SetText(bzKeyName);
                }
            }

            if (state == SND_STATE_ACTIVE)
            {
                m_EventState.SetText("ROUND " + round.ToString());
                m_EventState.SetColor(ARGB(255, 255, 255, 255));
                m_KeyText.Show(false);
                m_KeyImage.Show(false);
            }

            if (state == SND_STATE_ARMED)
            {
                m_EventState.SetText("BOMB ARMED");
                m_EventState.SetColor(ARGB(255, 255, 0, 0));
                m_KeyText.Show(false);
                m_KeyImage.Show(false);
            }

            if (state == SZ_STATE_LEAVING)
            {
                m_EventState.SetText("LEAVING SAFE ZONE");
                m_EventState.SetColor(ARGB(255, 255, 0, 0));
                m_KeyText.Show(false);
                m_KeyImage.Show(false);
            }
        }

        else m_EventState.Show(false);
    }

// =============== BUY ZONE ===============

    void InitSZCheck()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.CheckSZStatus, 50, true);
    }

    void StopSZCheck()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CheckSZStatus);
    }

    void CheckSZStatus()
    {
        m_SafezoneStatus = GetSafezoneStatus();
        if (m_SafezoneStatus == SZ_INSIDE || m_SafezoneStatus == BZ_INSIDE || m_SafezoneStatus == SPAWN_PROTECTION)
        {
            HideTimer();
            ShowState(true, BZ_STATE_TRUE);
        }
        else if (m_SafezoneStatus == SZ_LEAVING)
        {
            if (!m_IsTimerActive)
            {
                SetTime(10);
                ShowState(true, SZ_STATE_LEAVING);
            }
        }
        else
        {
            HideTimer();
            ShowState(false, NO_ZONE);
        }
    }

// =============== TIMER ===============

    void HideTimer()
    {
        m_IsTimerActive = false;
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.TimerUpdate);
        m_Timer.Show(false);
    }

    void SetTime(float durationInSeconds)
    {
        m_Timer.Show(true);
        m_EndTime = GetGame().GetTime() / 1000.0 + durationInSeconds;
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.TimerUpdate, 1000, true);
        float remainingTime = m_EndTime - GetGame().GetTime() / 1000.0;
        UpdateTimerText(remainingTime);
    }

    void TimerUpdate()
    {
        float currentTime = GetGame().GetTime() / 1000.0;
        float remainingTime = m_EndTime - currentTime;

        if (remainingTime <= 0)
        {
            HideTimer();
            return;
        }

        UpdateTimerText(remainingTime);
        m_IsTimerActive = true;
    }

    void UpdateTimerText(float time)
    {
        int minutes = Math.Floor(time / 60);
        int seconds = Math.Floor(time - (minutes * 60));

        string secondsString;
        if (seconds < 10)
            secondsString = "0" + seconds.ToString();
        else secondsString = seconds.ToString();

        string formattedTime = minutes.ToString() + ":" + secondsString;
        m_Timer.SetText(formattedTime);
        m_Timer.SetColor(GetTimerARGB(time));
    }
}