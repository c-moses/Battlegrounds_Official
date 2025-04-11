modded class GameConstants
{
    const int	STAMINA_DRAIN_SWIM_FAST_PER_SEC = 0;
    const float	STAMINA_DRAIN_JUMP = 15;
	const float	STAMINA_DRAIN_VAULT = 15;
	const float	STAMINA_DRAIN_CLIMB = 30;
    const float STAMINA_JUMP_THRESHOLD = 5;
	const float STAMINA_VAULT_THRESHOLD = 5;
	const float STAMINA_CLIMB_THRESHOLD = 5;
    const float STAMINA_REGEN_COOLDOWN_DEPLETION = 0.0;
	const float STAMINA_REGEN_COOLDOWN_EXHAUSTION = 0.0;
    const float STAMINA_SYNC_RATE = 0.25;

    const float CAR_DAMAGE_REDUCTION_FACTOR = 0.2;

	const int CARS_FLUIDS_TICK 		= 5;
	const int CARS_LEAK_TICK_MIN 	= 0.02;
	const int CARS_LEAK_TICK_MAX 	= 0.05;
	const int CARS_LEAK_THRESHOLD	= 0.5;

    const float CARS_CONTACT_DMG_THRESHOLD = 750.0;
	const float CARS_CONTACT_DMG_MIN = 75.0;
	const float CARS_CONTACT_DMG_KILLCREW = 2400.0;
}

const float EVENT_TICK = 1.0;

const string MODE_CLEANUP = "cleanup";
const string MODE_DAYZ = "dayz";
const string MODE_KOTH = "koth";
const string MODE_SND = "snd";

const string RED_TEAM = "Red Team";
const string BLUE_TEAM = "Blue Team";

const string TEAM_BLUE = "BlueTeamID";
const string TEAM_RED = "RedTeamID";
const string TEAM_GREEN = "GreenTeamID";
const string TEAM_ORANGE = "OrangeTeamID";
const string TEAM_YELLOW = "YellowTeamID";
const string TEAM_PINK = "PinkTeamID";
const string NO_TEAM = "TeamID";

const int POS_STANDARD = 1380;
const int POS_UNDERGROUND_ENOCH = 1381;

const int REQUEST_TEAMSELECT = 1390;
const int CLOSE_VOTE = 1391;
const int REQUEST_VOTE = 1392;

const int SPAWN_PROTECTION = 2955;
const int NO_ZONE = 2956;
const int SZ_INSIDE = 2957;
const int SZ_LEAVING = 2958;
const int BZ_INSIDE = 2959;

const int NOTIFY_LENGTH = 4;

const int UI_CURRENCY = 3970;
const int UI_TEAMSELECT = 3971;
const int UI_EVENTTIMER = 3972;
const int UI_KOTH = 3973;
const int UI_COMPASS = 3974;
const int UI_SND = 3975;

const int MENU_LEADERBOARD = 3454;
const int MENU_TRANSFER = 4543;
const int MENU_SCORE = 5194;
const int MENU_BUY = 9413;
const int MENU_GAMEMASTER = 9414;

const int BUYMENU_RESTRICT_NONE = 7080;
const int BUYMENU_RESTRICT_SAFEZONE = 7081;
const int BUYMENU_RESTRICT_SND_MID = 7082;
const int BUYMENU_RESTRICT_SND_HIGH = 7083;

const int STATE_CLEANUP = 9910;
const int KOTH_STATE_INIT = 8380;
const int SND_STATE_INIT = 6781;
const int SND_STATE_ACTIVE_BUY = 6782;
const int SND_STATE_ACTIVE = 6783;
const int SND_STATE_ARMED = 6784;
const int SND_STATE_COMPLETED = 6785;

const int BZ_STATE_TRUE = 8785;
const int SZ_STATE_LEAVING = 8786;

const string NOTIFY_LEAVE_TEAM = "You will be removed from team next respawn";
const string NOTIFY_SND_TELEPORT = "You will be teleported start of next round";
const string NOTIFY_SND_START = "Round started";
const string NOTIFY_SND_ARMED = " armed the bomb";
const string NOTIFY_SND_DISARMED = " disarmed the bomb";
const string NOTIFY_SND_DETONATED = " destroyed the objective";
const string NOTIFY_SND_ELIMINATED = " has been eliminated";
const string NOTIFY_SND_TIME_LIMIT = " held off the attackers";
const string NOTIFY_SND_SWITCH = "Switching sides, prepare for the next round";
const string ICON_SND_GENERAL = "set:ccgui_enforce image:HudUserMarker";
const string ICON_SND_SWITCH = "set:dayz_gui image:icon_refresh";
const string ICON_SND_ARMED = "set:ccgui_enforce image:Icon40Emergency";
const string ICON_SND_DISARMED = "set:ccgui_enforce image:HudRepair";
const string ICON_SND_DETONATED = "set:dayz_inventory image:explosive";
const string ICON_SND_ELIMINATED = "set:dayz_gui image:iconSkull";
const string ICON_SND_TIME_LIMIT = "set:ccgui_enforce image:HudDefend";

const string NOTIFY_PROMOTION = "You have been promoted!";
const string ICON_PROMOTION = "set:ccgui_enforce image:Icon40Turbo";

const string ICON_SZ = "set:ccgui_enforce image:Icon40Defensive";
const string ICON_BZ = "set:ccgui_enforce image:Icon40Offensive";
const string ICON_FOLLOW = "set:dayz_gui image:icon_player";

const string NOTIFY_SZ_ENTERED = "You have entered the safe zone";
const string NOTIFY_SZ_EXITED = "You have left the safe zone";
const string NOTIFY_BZ_ENTERED = "You have entered a buy zone";
const string NOTIFY_BZ_EXITED = "You have left the buy zone";

const string NOTIFY_BARTER_SUCCESS = "Barter success";
const string NOTIFY_BARTER_SUCCESS_SENT = " and XP earned";

const string NOTIFY_TRANSFER_SUCCESS = "Transfer success";
const string NOTIFY_TRANSFER_SUCCESS_SENT = " successfully sent to user";
const string NOTIFY_TRANSFER_FAILED = "Transfer failed";
const string NOTIFY_TRANSFER_FAILED_NOPLAYER = "User has not played on the server";
const string NOTIFY_TRANSFER_FAILED_INVALID = "Invalid entry";
const string NOTIFY_TRANSFER_FAILED_INSUFFICIENT = "Insufficient funds";

const string NOTIFY_FOLLOWER = "You are following: ";
const string NOTIFY_FOLLOWED = "You are being followed by: ";
const string NOTIFY_FOLLOW_MSG = "For the next 30 seconds";