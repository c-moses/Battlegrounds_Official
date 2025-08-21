Battlegrounds DayZ mod (Steam workshop):

Init occurs from MissionServer.c -> OnInit()

Manager_State.c will always know the game state
Manager_Teams.c will always know all things pertaining to teams

Manager_KOTH.c OR Manager_SnD.c will be the game mode- never both at once. These closely work with Manager_State and Manager_Teams

Layers must always inherit from the game modes base class
If you're creating new layers, class names must match the naming conventions/numeric prefix per mode & map, 
Example class name: Chernarus KOTH = 'class koth_186' .. the 86 isn't important here, but the 1 is, as it signals Chernarus- without it, the mod won't know where to put safezones, traders etc.. so Chernarus = 1, Livonia = 2, Sakhal = 3
