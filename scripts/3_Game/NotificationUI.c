modded class NotificationUI
{
    override void UpdateOffset()
    {
        UIScriptedMenu menu = UIScriptedMenu.Cast(GetGame().GetUIManager().GetMenu());
        if (!menu)
            return;

        Widget layoutRoot = menu.GetLayoutRoot();
        if (!layoutRoot)
            return;

        Widget expNotification = menu.GetLayoutRoot().FindAnyWidget("notification_root");
        if (!expNotification)
            return;

        if (expNotification.IsVisible())
        {
            if (!m_OffsetEnabled)
            {
                m_Root.GetPos(m_BackupPosX, m_BackupPosY);

                float x, y, w, h;
                m_Root.GetScreenPos(x, y);
                expNotification.GetScreenSize(w, h);

                m_Root.SetScreenPos(x, h);
                m_OffsetEnabled = true;
            }
        }
        else if (m_OffsetEnabled)
        {
            m_Root.SetPos(m_BackupPosX, m_BackupPosY);
            m_OffsetEnabled = false;
        }
    }
}