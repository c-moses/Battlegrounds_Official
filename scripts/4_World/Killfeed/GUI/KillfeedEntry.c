class KillfeedEntry
{
	private Widget root;
	private Widget background;
	private TextWidget murderName;
	private TextWidget targetName;
	private TextWidget dst;
	private ItemPreviewWidget murderWeapon;

	private ref KillfeedWrapper parent;

	private EntityAI localWeapon;

	void KillfeedEntry(KillfeedWrapper par, string mName, string mTeam, string tName, string tTeam, string wType, int dist, string msg, int type)
	{
		parent = par;
		if (type)
		{
			root = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/KillfeedEntry.layout", par.GetRoot());
			murderName = TextWidget.Cast(root.FindAnyWidget("MurderName"));
			targetName = TextWidget.Cast(root.FindAnyWidget("TargetName"));
			dst = TextWidget.Cast(root.FindAnyWidget("KillDst"));
			murderWeapon = ItemPreviewWidget.Cast(root.FindAnyWidget("MurderWeapon"));

			murderName.SetText(FormatNick(mName));
			targetName.SetText(FormatNick(tName));
            murderName.SetColor(GetTeamARGB(mTeam));
            targetName.SetColor(GetTeamARGB(tTeam));
			dst.SetText(String(dist.ToString() + "m"));

			SetWeapon(wType);
		}
		else
		{
			root = GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/KillfeedEntry2.layout", par.GetRoot());
			targetName = TextWidget.Cast(root.FindAnyWidget("TargetName"));
			murderName = TextWidget.Cast(root.FindAnyWidget("DeathMsg"));
			murderName.SetText(msg);
			targetName.SetText(FormatNick(tName));
            targetName.SetColor(GetTeamARGB(tTeam));
		}

		background = root.FindAnyWidget("Background");

		float width, height;
		width = GetFeedWidth();
		background.GetSize(null, height);
		background.SetSize(width, height);


		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Destroy, 10000, false);
	}

	string FormatNick(string source)
	{
		string result = source;
		if (result.Length() > 16)
			result = result.Substring(0, 12) + "...";
		return result;
	}

	void SetWeapon(string type)
	{
		EntityAI weapon = EntityAI.Cast(GetGame().CreateObject(type, vector.Zero, true));
		if (!weapon)
			return;
			
		murderWeapon.SetItem(weapon);
		murderWeapon.SetView(weapon.GetViewIndex());
		murderWeapon.SetModelOrientation("0 0 0");
		localWeapon = weapon;
	}

	void Destroy()
	{
		GetFeedWidth();
		parent.RemoveItem(this);
		root.Unlink();
		if (localWeapon)
			GetGame().ObjectDelete(localWeapon);
	}

	float GetFeedWidth()
	{
		float start, end, width;
		murderName.GetPos(null, start);
		targetName.GetPos(null, end);
		targetName.GetSize(width, null);
		width = start + end + width;

		Print("Width "+width);
		Print("start "+start);
		Print("end "+end);
		return width;
	}

    int GetTeamARGB(string teamID)
    {
        switch (teamID)
        {
            case TEAM_BLUE:
                return ARGB(255, 48, 98, 173);
            case TEAM_RED:
                return ARGB(255, 157, 0, 0);
            case TEAM_GREEN:
                return ARGB(255, 0, 89, 0);
            case TEAM_ORANGE:
                return ARGB(255, 179, 89, 0);
            case TEAM_YELLOW:
                return ARGB(255, 133, 136, 0);
            case TEAM_PINK:
                return ARGB(255, 151, 70, 149);
            default:
                return ARGB(255, 255, 255, 255);
        }
        return ARGB(255, 255, 255, 255);
    }
}