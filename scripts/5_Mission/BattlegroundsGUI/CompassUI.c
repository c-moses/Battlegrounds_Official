class CompassWidget extends UIHelper
{
	private Widget m_CompassWidget;
	private Widget m_ObjectiveWidget;
    private ImageWidget m_ObjectiveIconWidget;
	private TextWidget m_ObjectiveDistance;
	private Widget m_Objective2Widget;
    private ImageWidget m_Objective2IconWidget;
	private TextWidget m_Objective2Distance;
	private Widget m_BuyZoneWidget;
	private TextWidget m_BuyZoneIconWidget;
	private TextWidget m_BuyZoneDistance;
	
    private vector m_ObjectiveLocation;
	private vector m_Objective2Location;
	private ref array<vector> m_BuyZones = new array<vector>;
	private vector m_NearestBuyZone;
	private vector m_PreviousBuyZone;
	protected bool m_IsVisible = false;

	void~CompassWidget()
	{ 
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CompassUpdate);
	}

	override Widget Init()
	{
		layoutRoot 				= GetGame().GetWorkspace().CreateWidgets("Battlegrounds/GUI/layouts/Compass.layout");
		m_CompassWidget 		= Widget.Cast(layoutRoot.FindAnyWidget("Compass"));
		m_ObjectiveWidget 		= layoutRoot.FindAnyWidget("ObjectiveFrame");
        m_ObjectiveIconWidget 	= ImageWidget.Cast(layoutRoot.FindAnyWidget("ObjectiveIcon"));
		m_ObjectiveDistance 	= TextWidget.Cast(layoutRoot.FindAnyWidget("ObjectiveDistance"));
		m_Objective2Widget 		= layoutRoot.FindAnyWidget("Objective2Frame");
        m_Objective2IconWidget 	= ImageWidget.Cast(layoutRoot.FindAnyWidget("Objective2Icon"));
		m_Objective2Distance 	= TextWidget.Cast(layoutRoot.FindAnyWidget("Objective2Distance"));
		m_BuyZoneWidget 		= layoutRoot.FindAnyWidget("BuyZoneFrame");
		m_BuyZoneIconWidget 	= TextWidget.Cast(layoutRoot.FindAnyWidget("BuyZoneIcon"));
		m_BuyZoneDistance 		= TextWidget.Cast(layoutRoot.FindAnyWidget("BuyZoneDistance"));

		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.CompassUpdate, 16, true);
		SetID(UI_COMPASS);
		return layoutRoot;
	}

	void CloseUI()
    {
        if (layoutRoot)
        {
            layoutRoot.Show(false);
            layoutRoot.Unlink();
        }

        GetGame().GetUIManager().HideScriptedMenu(this);
    }

	void CleanupCompass()
	{
		m_ObjectiveWidget.Show(false);
		m_Objective2Widget.Show(false);
		m_BuyZoneWidget.Show(false);

		m_ObjectiveLocation = vector.Zero;
		m_Objective2Location = vector.Zero;
		m_BuyZones.Clear();
	}

	void ShowCompass(bool show)
    {
        if (show)
        {
            layoutRoot.Show(true);
            m_IsVisible = true;
        }
        else
        {
            layoutRoot.Show(false);
            m_IsVisible = false;
        }
    }

	void ToggleCompass()
	{
		if (m_IsVisible)
        {
            layoutRoot.Show(false);
            m_IsVisible = false;
        }
        else
        {
            layoutRoot.Show(true);
            m_IsVisible = true;
        }
	}

	void SetSnDCoords(vector siteA, vector siteB, int siteArmed)
	{
		m_ObjectiveLocation = siteA;
		m_Objective2Location = siteB;

		if (siteArmed == 0)
		{
			m_ObjectiveWidget.Show(true);
			m_Objective2Widget.Show(true);
		}
		else if(siteArmed == 1)
		{
			m_ObjectiveWidget.Show(true);
			m_Objective2Widget.Show(false);
		}
		else if(siteArmed == 2)
		{
			m_ObjectiveWidget.Show(false);
			m_Objective2Widget.Show(true);
		}
	}

	void SetKOTHCoords(vector loc)
    {
		m_ObjectiveWidget.Show(true);
		m_ObjectiveLocation = loc;
		m_Objective2Location = vector.Zero;
	}

	void SetBuyZones(array<vector> buyZones)
	{
		m_BuyZoneWidget.Show(true);
		m_BuyZones = buyZones;
	}

	void CompassUpdate()
	{
		if (layoutRoot && GetGame().GetPlayer())
		{
			float angle = CompassGetAngle();
			SetCompassPos(angle);

			m_PlayerPosition = GetPlayerPosition();

			if (m_ObjectiveLocation != vector.Zero)
			{
				float directionToObjective = GetDirectionToLocation(m_ObjectiveLocation);
				SetIconPos(directionToObjective, m_ObjectiveWidget);

				float objectiveDistance = vector.Distance(GetGame().GetPlayer().GetPosition(), m_ObjectiveLocation);
				FormatWidgetDistances(m_ObjectiveDistance, objectiveDistance);
			}
			else m_ObjectiveWidget.Show(false);

			if (m_Objective2Location != vector.Zero)
			{
				float directionToObjective2 = GetDirectionToLocation(m_Objective2Location);
				SetIconPos(directionToObjective2, m_Objective2Widget);

				float objective2Distance = vector.Distance(GetGame().GetPlayer().GetPosition(), m_Objective2Location);
				FormatWidgetDistances(m_Objective2Distance, objective2Distance);
			}
			else m_Objective2Widget.Show(false);

			UpdateNearestBuyZone();
			layoutRoot.Update();
		}
	}

	void UpdateNearestBuyZone()
	{
		if (m_PlayerPosition == vector.Zero || !m_BuyZones || m_BuyZones.Count() == 0) 
			return;

		float minDistance = float.MAX;
		vector nearestZone = vector.Zero;
		for (int i = 0; i < m_BuyZones.Count(); i++)
		{
			float distance = vector.Distance(m_PlayerPosition, m_BuyZones[i]);
			if (distance < minDistance)
			{
				minDistance = distance;
				nearestZone = m_BuyZones[i];
			}
		}

		if (nearestZone != m_NearestBuyZone && nearestZone != vector.Zero)
		{
			m_PreviousBuyZone = m_NearestBuyZone;
			m_NearestBuyZone = nearestZone;
		}

		if (m_NearestBuyZone != m_PreviousBuyZone)
		{
			float directionToBuyZone = GetDirectionToLocation(m_NearestBuyZone);
			SetIconPos(directionToBuyZone, m_BuyZoneWidget);
			m_BuyZoneWidget.Show(true);
		}

		float buyZoneDistance = vector.Distance(m_PlayerPosition, m_NearestBuyZone);
		FormatWidgetDistances(m_BuyZoneDistance, buyZoneDistance);
	}

	void FormatWidgetDistances(Widget widget, float distance)
	{
		TextWidget textWidget = TextWidget.Cast(widget);
		if (!textWidget) return;

		if (widget == m_Objective2Distance && distance > 999)
		{
			textWidget.SetText("");
			return;
		}

		string formattedDistance;
		if (distance < 1000)
		{
			formattedDistance = string.Format("%1m", Math.Floor(distance));
		}
		else
		{
			float distanceKm = distance / 1000;
			float roundedDistanceKm = Math.Round(distanceKm * 10) / 10;
			formattedDistance = string.Format("%1km", roundedDistanceKm);
		}

		textWidget.SetText(formattedDistance);
	}

	void SetIconPos(float direction, Widget iconWidget)
	{
		if (!iconWidget) return;
		float compassAngle = CompassGetAngle();
		float relativeAngle = compassAngle - direction;
		while (relativeAngle < -180) relativeAngle += 360;
		while (relativeAngle > 180) relativeAngle -= 360;
		float width, height, x, y;
		iconWidget.GetPos(x, y);
		iconWidget.GetSize(width, height);
		float center_offset = width / -2.0 + 0.5;
		float offset = relativeAngle * (center_offset / 180.0) + center_offset;
		iconWidget.SetPos(offset, y);
	}

    float GetDirectionToLocation(vector objectivePos)
    {
        vector dirToObjective = objectivePos - m_PlayerPosition;
        dirToObjective.Normalize();
        float angle = Math.Atan2(dirToObjective[0], dirToObjective[2]) * Math.RAD2DEG;
        return angle;
    }

	void SetCompassAlpha(float alpha)
	{
		if (layoutRoot != null)
			layoutRoot.SetAlpha(alpha);
	}

	string CompassGetHeading(float angle)
	{
		float rounded = Math.Round(5 *Math.Round(angle / 5.0));
		return rounded.ToString();
	}

	void SetCompassPos(float angle)
	{
		float width, height, x, y;
		m_CompassWidget.GetPos(x, y);
		m_CompassWidget.GetSize(width, height);

		float center_offset = width / -2.0 + 0.5;

		if (angle > 180)
			angle = angle - 360;

		float offset = angle *(center_offset / 180.0) + center_offset;

		m_CompassWidget.SetPos(offset, y);
	}

	float CompassGetAngle()
	{
		vector direction = GetGame().GetCurrentCameraDirection();
		float angle = direction.VectorToAngles()[0];
		return angle;
	}
}