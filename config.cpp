class CfgPatches
{
	class Battlegrounds
	{
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Scripts",
            "DZ_Radio",
			"DZ_Gear_Medical",
			"DZ_Gear_Consumables",
			"DZ_Gear_Tools",
			"DZ_Gear_Optics",
			"DZ_Gear_Camping",
			"DZ_Weapons_Firearms",
			"DZ_Weapons_Magazines",
			"DZ_Weapons_Muzzles",
			"DZ_Weapons_Melee",
			"DZ_Surfaces",
			"DZ_Structures",
			"DZ_Sounds_Effects",
			"BattlegroundsUtil"
		};
		units[]={};
		weapons[]={};
	};
};
class CfgMods
{
	class Battlegrounds
	{
		dir="Battlegrounds";
		author="Moses";
		inputs="BattlegroundsUtil/Inputs.xml";
		type="mod";
		dependencies[]=
		{
			"Game",
			"World",
            "Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				files[]=
				{
					"Battlegrounds/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				files[]=
				{
					"Battlegrounds/scripts/4_World"
				};
			};
            class missionScriptModule
			{
				files[]=
				{
					"Battlegrounds/scripts/5_Mission"
				};
			};
		};
	};
};
class cfgVehicles
{
	class Inventory_Base;
	class CannabisSeedsPack: Inventory_Base
	{
		displayName="Weed Seed Pack";
		descriptionShort="Can be grown and sold for cash/XP";
		class Horticulture
		{
			ContainsSeedsType="CannabisSeeds";
			ContainsSeedsQuantity=10;
		};
	};
	class SeedBase;
	class CannabisSeeds: SeedBase
	{
		displayName="Weed Seed";
		descriptionShort="Can be grown and sold for cash/XP";
	};
	class Edible_Base;
	class Cannabis: Edible_Base
	{
		displayName="Weed";
		descriptionShort="Can be sold for cash/XP";
	};
	class Clothing_Base;
	class Clothing: Clothing_Base {};
	class MilitaryBelt: Clothing
	{
		scope=2;
		displayName="Belt";
		descriptionShort="";
		visibilityModifier=0.85;
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.0784314,0.0784314,0.0784314,1.0,CO)",
			"#(argb,8,8,3)color(0.0784314,0.0784314,0.0784314,1.0,CO)",
			"#(argb,8,8,3)color(0.0784314,0.0784314,0.0784314,1.0,CO)",
			"#(argb,8,8,3)color(0.0784314,0.0784314,0.0784314,1.0,CO)",
			"#(argb,8,8,3)color(0.0784314,0.0784314,0.0784314,1.0,CO)",
			"#(argb,8,8,3)color(0.0784314,0.0784314,0.0784314,1.0,CO)",
			"#(argb,8,8,3)color(0.0784314,0.0784314,0.0784314,1.0,CO)",
			"#(argb,8,8,3)color(0.0784314,0.0784314,0.0784314,1.0,CO)",
			"#(argb,8,8,3)color(0.0784314,0.0784314,0.0784314,1.0,CO)"
		};
		attachments[]=
		{
			"MedicalBandage",
			"Knife",
			"Belt_Right"
		};
		weight=5;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
		};
	};
	class GasMask: Clothing
	{
		scope=2;
		displayName="Gas Mask";
		descriptionShort="$STR_CfgVehicles_GasMask1";
		model="\DZ\characters\masks\GasMask_g.p3d";
		itemSize[]={2,2};
		varQuantityInit=150;
		varQuantityMin=0;
		varQuantityMax=150;
		inventorySlot[]=
		{
			"Mask"
		};
	};
	class GhillieSuit_ColorBase: Clothing
	{
		weight=500;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=500;
				};
			};
		};
	};
	class Armband_ColorBase: Clothing 
	{
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100000;
				};
			};
		};
	}
	class Armband_Green;
	class GreenTeamID: Armband_Green
	{
		displayName="Green Team ID";
		descriptionShort="$STR_CfgVehicles_Armband_ColorBase1";
	};
	class Armband_Blue;
	class BlueTeamID: Armband_Blue
	{
		displayName="Blue Team ID";
		descriptionShort="$STR_CfgVehicles_Armband_ColorBase1";
	};
	class Armband_Red;
	class RedTeamID: Armband_Red
	{
		displayName="Red Team ID";
		descriptionShort="$STR_CfgVehicles_Armband_ColorBase1";
	};
	class Armband_Orange;
	class OrangeTeamID: Armband_Orange
	{
		displayName="Orange Team ID";
		descriptionShort="$STR_CfgVehicles_Armband_ColorBase1";
	};
	class Armband_Yellow;
	class YellowTeamID: Armband_Yellow
	{
		displayName="Yellow Team ID";
		descriptionShort="$STR_CfgVehicles_Armband_ColorBase1";
	};
	class Armband_Pink;
	class PinkTeamID: Armband_Pink
	{
		displayName="Pink Team ID";
		descriptionShort="$STR_CfgVehicles_Armband_ColorBase1";
	};
	class Inventory_Base;
	class SnD_Bomb: Inventory_Base
	{
		scope=2;
		displayName="SnD Bomb";
		descriptionShort="";
		model="\DZ\weapons\explosives\ImprovisedExplosive.p3d";
		isMeleeWeapon=1;
		rotationFlags=63;
		itemSize[]={3,3};
		weight=100;
        class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100000;
					healthLevels[]=
					{
						{
							1,
							{
								"DZ\weapons\explosives\data\ied.rvmat",
								"DZ\gear\containers\data\Protector_Case.rvmat"
							}
						},
						{
							0.69999999,
							{
								"DZ\weapons\explosives\data\ied.rvmat",
								"DZ\gear\containers\data\Protector_Case.rvmat"
							}
						},
						{
							0.5,
							{
								"DZ\weapons\explosives\data\ied_damage.rvmat",
								"DZ\gear\containers\data\Protector_Case_damage.rvmat"
							}
						},
						{
							0.30000001,
							{
								"DZ\weapons\explosives\data\ied_damage.rvmat",
								"DZ\gear\containers\data\Protector_Case_damage.rvmat"
							}
						},
						{
							0,
							{
								"DZ\weapons\explosives\data\ied_destruct.rvmat",
								"DZ\gear\containers\data\Protector_Case_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType="textile";
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem_Light
				{
					soundSet="pickUpCourierBag_Light_SoundSet";
					id=796;
				};
				class pickUpItem
				{
					soundSet="pickUpCourierBag_SoundSet";
					id=797;
				};
			};
		};
	};
	class SnD_Site: Inventory_Base
	{
		scope=2;
		displayName="SnD Site";
		descriptionShort="";
		model="DZ\structures\Military\Misc\Misc_SupplyBox3.p3d";
		weight=30000;
		physLayer="item_large";
		simulation="inventoryItem";
		carveNavmesh=1;
		canBeDigged=0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100000;
				};
			};
		};
	};
	class SnD_SiteA: SnD_Site {};
	class SnD_SiteB: SnD_Site {};
	class FirstAidKit_BG: Inventory_Base
	{
		scope=2;
		displayName="First Aid Kit";
		descriptionShort="Fixes all wounds and speeds up recovery";
		model="\dz\gear\containers\FirstAidKit.p3d";
		isMeleeWeapon=1;
		varQuantityInit=2;
		varQuantityMin=0;
		varQuantityMax=2;
		quantityBar=1;
		varQuantityDestroyOnMin=1;
		rotationFlags=63;
		itemSize[]={2,2};
		weight=100;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\gear\containers\data\FirsAidKit.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\gear\containers\data\FirsAidKit.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\gear\containers\data\FirsAidKit_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\gear\containers\data\FirsAidKit_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\gear\containers\data\FirsAidKit_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType="textile";
		class AnimEvents
		{
			class SoundWeapon
			{
				class Bandage_loop1
				{
					soundSet="Bandage_loop_SoundSet";
					id=201;
				};
				class Bandage_loop2
				{
					soundSet="Bandage_loop_SoundSet";
					id=202;
				};
				class Bandage_loop3
				{
					soundSet="Bandage_loop_SoundSet";
					id=203;
				};
				class Bandage_start
				{
					soundSet="Bandage_start_SoundSet";
					id=204;
				};
				class Bandage_end
				{
					soundSet="Bandage_end_SoundSet";
					id=205;
				};
				class pickUpItem_Light
				{
					soundSet="pickUpCourierBag_Light_SoundSet";
					id=796;
				};
				class pickUpItem
				{
					soundSet="pickUpCourierBag_SoundSet";
					id=797;
				};
			};
		};
	};
	class Epinephrine: Inventory_Base
	{
		scope=2;
		displayName="Epinephrine";
		descriptionShort="Injection gives a few minutes of unlimited stamina";
	};
	class Morphine: Inventory_Base
	{
		scope=2;
		displayName="Morphine";
		descriptionShort="Injection will speed up recovery of broken bones and overall health";
	};
	class BloodBagIV: Inventory_Base
	{
		scope=2;
		displayName="Blood Kit";
		descriptionShort="O-Negative Blood.";
		model="\dz\gear\medical\BloodBag_Full_IV.p3d";
		debug_ItemCategory=7;
		rotationFlags=17;
		itemSize[]={2,2};
		weight=10;
		weightPerQuantityUnit=1;
		quantityBar=1;
		stackedUnit="ml";
		varQuantityInit=500;
		varQuantityMin=0;
		varQuantityMax=500;
		varQuantityDestroyOnMin=1;
		varLiquidTypeInit=2;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=500;
				};
			};
		};
	};
	class Transmitter_Base;
    class PersonalRadio: Transmitter_Base
	{
		scope=2;
		itemSize[]={1,2};
		simulation="itemTransmitter";
		inputRange=8;
		range=20000;
		class EnergyManager
		{
			hasIcon=1;
			autoSwitchOff=1;
			energyUsagePerSecond=0.001;
			plugType=1;
			attachmentAction=1;
			wetnessExposure=0.1;
		};
	};
	class CamoNet: Inventory_Base
	{
		scope=2;
		displayName="$STR_CfgVehicles_CamoNet0";
		descriptionShort="$STR_CfgVehicles_CamoNet1";
		model="\dz\gear\camping\camo_net_p.p3d";
		itemSize[]={4,2};
		weight=1;
	};
    class ItemOptics;
    class NVGoggles: ItemOptics
	{
		scope=2;
		displayName="NVG";
		class EnergyManager
		{
			hasIcon=1;
			energyUsagePerSecond=0.001;
			plugType=1;
			attachmentAction=1;
		};
	};
	class ItemSuppressor;
	class UniversalSuppressorBase: ItemSuppressor
	{
		scope=0;
		displayName="Suppressor";
		descriptionShort="A suppressor that'll fit any barrel of any gun";
		model="\dz\weapons\attachments\muzzle\suppressor_556.p3d";
		rotationFlags=17;
		reversed=0;
		weight=1000;
		itemSize[]={3,1};
		itemModelLength=0.15000001;
		barrelArmor=2250;
		lootCategory="Attachments";
		lootTag[]=
		{
			"Military_west"
		};
        inventorySlot[]=
		{
			"weaponMuzzleM4",
			"weaponMuzzleMosin",
			"weaponMuzzleAK",
			"suppressorImpro"
		};
		simulation="ItemSuppressor";
		dispersionModifier=-0.00050000002;
		noiseShootModifier=-0.80000001;
		dispersionCondition="true";
		recoilModifier[]={0.94999999,0.94999999,0.94999999};
		swayModifier[]={1.05,1,1};
		soundIndex=1;
		muzzlePos="usti hlavne";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=2;
					healthLevels[]=
					{
						{
							1,
							
							{
								"DZ\weapons\attachments\data\suppressor556.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\attachments\data\suppressor556.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\attachments\data\suppressor556_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\attachments\data\suppressor556_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\attachments\data\suppressor556_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class Particles
		{
			class OnFire
			{
				class MuzzleFlash
				{
					overrideParticle="weapon_shot_fnx_02";
					onlyWithinHealthLabel[]={0,3};
				};
			};
			class OnOverheating
			{
				shotsToStartOverheating=3;
				maxOverheatingValue=10;
				overheatingDecayInterval=1;
				class SmokeTrail
				{
					overrideParticle="smoking_barrel_small";
					onlyWithinRainLimits[]={0,0.2};
				};
				class SmokingBarrelHotSteamSmall
				{
					overrideParticle="smoking_barrel_steam_small";
					positionOffset[]={0,0.1,0};
					onlyWithinOverheatLimits[]={0,1};
					onlyWithinRainLimits[]={0.2,1};
				};
			};
		};
	};
    class UniversalSuppressorM4: UniversalSuppressorBase
    {
		scope=2;
		displayName="Suppressor";
		descriptionShort="A suppressor that'll fit any barrel of any gun";
	};
	class UniversalSuppressorAK: UniversalSuppressorBase
    {
		scope=2;
		displayName="Suppressor";
		descriptionShort="A suppressor that'll fit any barrel of any gun";
		model="\dz\weapons\attachments\muzzle\suppressor_545.p3d";
		class Particles
		{
			class OnFire
			{
				class MuzzleFlashAssault
				{
					overrideParticle="weapon_shot_akm_02";
					onlyWithinHealthLabel[]={0,3};
				};
			};
			class OnOverheating
			{
				shotsToStartOverheating=2;
				maxOverheatingValue=20;
				overheatingDecayInterval=1;
				class SmokingBarrel
				{
					overrideParticle="smoking_barrel_small";
					onlyWithinRainLimits[]={0,0.2};
				};
				class SmokingBarrelHotSteamSmall
				{
					overrideParticle="smoking_barrel_steam_small";
					positionOffset[]={0,0.1,0};
					onlyWithinRainLimits[]={0.2,1};
				};
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=4;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\weapons\attachments\data\tgpa.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\attachments\data\tgpa.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\attachments\data\tgpa_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\attachments\data\tgpa_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\attachments\data\tgpa_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class ReflexOptic: ItemOptics
	{
		scope=2;
		displayName="Red Dot Sight";
	};
	class KobraOptic: ItemOptics
	{
		scope=2;
		displayName="Kobra Sight";
	};
	class AcogOptic: ItemOptics
	{
		scope=2;
		displayName="4x Acog";
	};
	class AcogOptic_6x: ItemOptics
	{
		scope=2;
		displayName="6x Acog";
	};
	class M18SmokeGrenade_Green;
	class BGSmokeGreen : M18SmokeGrenade_Green
	{
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100000;
				};
			};
		};
	};
	class M18SmokeGrenade_Red;
	class BGSmokeRed : M18SmokeGrenade_Red
	{
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100000;
				};
			};
		};
	};
	class Stone;
	class Rock: Stone
	{
		scope=2;
	};
	class FireworksLauncher;
	class BGFireworks: FireworksLauncher {};
	class Container_Base;
	class TentBase;
	class MediumTent: TentBase
	{
		scope=2;
		displayName="Medium Tent";
		descriptionShort="600 slots, equipped with a camo net.";
		weight=200;
		itemSize[]={5,3};
		itemsCargoSize[]={10,60};
		itemBehaviour=2;
		attachments[]=
		{
			"CamoNet"
		};
		class GUIInventoryAttachmentsProps
		{
			class Attachments
			{
				attachmentSlots[]=
				{
					"CamoNet"
				};
			};
		};
	};
	class LargeTent: TentBase
	{
		scope=2;
		displayName="Large Tent";
		descriptionShort="800 slots, equipped with a camo net.";
		weight=400;
		itemSize[]={6,3};
		itemsCargoSize[]={10,80};
		itemBehaviour=2;
		attachments[]=
		{
			"CamoNet"
		};
		class GUIInventoryAttachmentsProps
		{
			class Attachments
			{
				attachmentSlots[]=
				{
					"CamoNet"
				};
			};
		};
	};
	class CarTent: TentBase
	{
		scope=2;
		displayName="Car Tent";
		descriptionShort="1000 slots, equipped with a camo net.";
		weight=600;
		itemSize[]={7,3};
		itemsCargoSize[]={10,100};
		itemBehaviour=2;
		attachments[]=
		{
			"CamoNet"
		};
		class GUIInventoryAttachmentsProps
		{
			class Attachments
			{
				attachmentSlots[]=
				{
					"CamoNet"
				};
			};
		};
	};
	class BattlegroundsRewardCrate: Container_Base
	{
		scope=2;
		displayName="Rewards";
		descriptionShort="";
		model="DZ\structures\Military\Misc\Misc_SupplyBox3.p3d";
		attachments[]=
		{
			"Headgear",
			"Vest",
			"Back"
		};
		itemsCargoSize[]={10,10};
		weight=30000;
		physLayer="item_large";
		simulation="inventoryItem";
		carveNavmesh=1;
		canBeDigged=0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100000;
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[]={10,10};
			openable=0;
			allowOwnedCargoManipulation=1;
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class movement
				{
					soundSet="seachest_movement_SoundSet";
					id=1;
				};
				class pickUpItem_Light
				{
					soundSet="pickUpSeaChest_SoundSet";
					id=796;
				};
				class pickUpItem
				{
					soundSet="pickUpSeaChest_SoundSet";
					id=797;
				};
				class drop
				{
					soundset="seachest_drop_SoundSet";
					id=898;
				};
			};
		};
	};
	class HouseNoDestruct;
    class BattlegroundsTeleporterPanel_Base: HouseNoDestruct
	{
		scope=1;
		model="DZ\structures\Furniture\radar_equipment\radar_panel_flat.p3d";
        class Doors
		{
			class doorOpen
			{
				displayName="Door1";
				component="doorOpen";
				soundPos="door1_action";
				animPeriod=1;
				initPhase=0;
				initOpened=0;
				soundOpen="";
				soundClose="";
				soundLocked="";
				soundOpenABit="";
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100000;
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
			};
			class DamageZones
			{
				class twin1
				{
					class Health
					{
						hitpoints=100000;
						transferToGlobalCoef=0;
					};
					componentNames[]=
					{
						"door1"
					};
					fatalInjuryCoef=-1;
					class ArmorType
					{
						class Projectile
						{
							class Health
							{
								damage=3;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
						class Melee
						{
							class Health
							{
								damage=5;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
						class FragGrenade
						{
							class Health
							{
								damage=3;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
					};
				};
			};
		};
	};
	class BattlegroundsTeleporterPanel_V1: BattlegroundsTeleporterPanel_Base
	{
		scope=1;
	}
	class BattlegroundsLock: Inventory_Base
	{
		scope=2;
		displayName="Battlegrounds Lock";
		descriptionShort="";
		model="\DZ\gear\camping\combination_lock4.p3d";
		itemSize[]={2,2};
		rotationFlags=34;
		weight=350;
		soundImpactType="metal";
		inventorySlot[]=
		{
			"BattlegroundsLock"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1500;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\gear\camping\data\combilock4_metal.rvmat",
								"DZ\gear\camping\data\combination_lock_chain.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\gear\camping\data\combilock4_metal.rvmat",
								"DZ\gear\camping\data\combination_lock_chain.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\gear\camping\data\combilock4_metal_damage.rvmat",
								"DZ\gear\camping\data\combination_lock_chain_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\gear\camping\data\combilock4_metal_damage.rvmat",
								"DZ\gear\camping\data\combination_lock_chain_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\gear\camping\data\combilock4_metal_destruct.rvmat",
								"DZ\gear\camping\data\combination_lock_chain_destruct.rvmat"
							}
						}
					};
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
			};
		};
	};
	class GreenTeamLock: BattlegroundsLock
	{
		scope=2;
		displayName="Green Team Lock";
		descriptionShort="When attached, only green team can open.";
	};
	class BlueTeamLock: BattlegroundsLock
	{
		scope=2;
		displayName="Blue Team Lock";
		descriptionShort="When attached, only blue team can open.";
	};
	class RedTeamLock: BattlegroundsLock
	{
		scope=2;
		displayName="Red Team Lock";
		descriptionShort="When attached, only red team can open.";
	};
	class OrangeTeamLock: BattlegroundsLock
	{
		scope=2;
		displayName="Orange Team Lock";
		descriptionShort="When attached, only orange team can open.";
	};
	class CombinationLock: Inventory_Base
	{
		scope=2;
		itemSize[]={2,2};
		weight=10;
	};
	class CarScript;
	class SimulationModule;
	class Axles;
	class Wheels;
	class Front;
	class Right;
	class Left;
	class Rear;
	class Offroad_02;
    class Offroad_SnD: Offroad_02
    {};
	class OffroadHatchback: CarScript
	{
		class SimulationModule: SimulationModule
		{
			class Steering
			{
				maxSteeringAngle=35;
				increaseSpeed[]={0,40,30,20,100,5};
				decreaseSpeed[]={0,80,60,40,90,20};
				centeringSpeed[]={0,0,15,25,60,40,100,60};
			};
			class Throttle
			{
				reactionTime=0.89999998;
				defaultThrust=0.85000002;
				gentleThrust=0.69999999;
				turboCoef=4;
				gentleCoef=0.75;
			};
			class Brake
			{
				pressureBySpeed[]={0,0.85000002,10,0.69999999,20,0.5,40,0.40000001,60,0.43000001,80,0.46000001,100,0.51999998,120,0.69999999};
				reactionTime=0.2;
				driverless=0.1;
			};
			class Aerodynamics
			{
				frontalArea=2.1800001;
				dragCoefficient=0.56;
			};
			drive="DRIVE_AWD";
			class Engine
			{
				torqueCurve[]={650,0,750,50,1400,100,3400,140,5400,110,8000,0}; // torqueCurve[]={650,0,750,40,1400,80,3400,114,5400,95,8000,0};
				inertia=0.11;
				frictionTorque=100;
				rollingFriction=2.5;
				viscousFriction=1;
				rpmIdle=800;
				rpmMin=900;
				rpmClutch=1400;
				rpmRedline=6000;
			};
			class Clutch
			{
				maxTorqueTransfer=240;
				uncoupleTime=0.30000001;
				coupleTime=0.44999999;
			};
			class Gearbox
			{
				type="GEARBOX_MANUAL";
				reverse=3.526;
				ratios[]={3.6670001,2.0999999,1.3609999,1};
			};
			class CentralDifferential
			{
				ratio=1.5;
				type="DIFFERENTIAL_LOCKED";
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxBrakeTorque=2000;
					maxHandbrakeTorque=2500;
					wheelHubMass=5;
					wheelHubRadius=0.15000001;
					class Differential
					{
						ratio=4.0999999;
						type="DIFFERENTIAL_OPEN";
					};
					class Suspension
					{
						stiffness=40000;
						compression=2100;
						damping=7500;
						travelMaxUp=0.088200003;
						travelMaxDown=0.083300002;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper="damper_1_1";
							inventorySlot="NivaWheel_1_1";
						};
						class Right: Right
						{
							animDamper="damper_2_1";
							inventorySlot="NivaWheel_2_1";
						};
					};
				};
				class Rear: Rear
				{
					maxBrakeTorque=1000;
					maxHandbrakeTorque=2500;
					wheelHubMass=5;
					wheelHubRadius=0.15000001;
					class Differential
					{
						ratio=4.0999999;
						type="DIFFERENTIAL_OPEN";
					};
					class Suspension
					{
						stiffness=40000;
						compression=2200;
						damping=7600;
						travelMaxUp=0.1587;
						travelMaxDown=0.1059;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper="damper_1_2";
							inventorySlot="NivaWheel_1_2";
						};
						class Right: Right
						{
							animDamper="damper_2_2";
							inventorySlot="NivaWheel_2_2";
						};
					};
				};
			};
		};
	};
	class Hatchback_02: CarScript
	{
		class SimulationModule: SimulationModule
		{
			class Steering
			{
				maxSteeringAngle=35;
				increaseSpeed[]={0,50,60,30,100,15};
				decreaseSpeed[]={0,90,60,45,100,20};
				centeringSpeed[]={0,0,15,27,60,45,100,63};
			};
			class Throttle
			{
				reactionTime=0.85000002;
				defaultThrust=0.69999999;
				gentleThrust=0.40000001;
				turboCoef=2.2;
				gentleCoef=0.5;
			};
			class Brake
			{
				pressureBySpeed[]={0,0.5,10,0.40000001,20,0.30000001,40,0.28,60,0.30000001,80,0.38,100,0.41999999,120,0.47999999,150,0.60000002};
				reactionTime=0.2;
				driverless=0.1;
			};
			class Aerodynamics
			{
				frontalArea=2.1500001;
				dragCoefficient=0.51999998;
				downforceCoefficient=0.0099999998;
				downforceOffset[]={0,0.80000001,-0.69999999};
			};
			drive="DRIVE_FWD";
			class Engine
			{
				torqueCurve[]={850,0,1150,110,2000,155,4400,230,5800,190,7500,0}; // torqueCurve[]={850,0,1150,90,2000,130,4400,186,5800,165,7500,0};
				inertia=0.20999999;
				frictionTorque=150;
				rollingFriction=2.5;
				viscousFriction=1;
				rpmIdle=1250;
				rpmMin=1500;
				rpmClutch=2000;
				rpmRedline=6250;
			};
			class Clutch
			{
				maxTorqueTransfer=360;
				uncoupleTime=0.25;
				coupleTime=0.34999999;
			};
			class Gearbox
			{
				type="GEARBOX_MANUAL";
				reverse=3.1670001;
				ratios[]={3.4549999,2.118,1.444,1.1289999,0.912};
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxBrakeTorque=1800;
					wheelHubMass=5;
					wheelHubRadius=0.125;
					class Differential
					{
						ratio=3.6670001;
						type="DIFFERENTIAL_OPEN";
					};
					class Suspension
					{
						stiffness=27000;
						compression=2000;
						damping=7500;
						travelMaxUp=0.16;
						travelMaxDown=0.16;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper="damper_1_1";
							inventorySlot="Hatchback_02_Wheel_1_1";
						};
						class Right: Right
						{
							animDamper="damper_2_1";
							inventorySlot="Hatchback_02_Wheel_2_1";
						};
					};
				};
				class Rear: Rear
				{
					maxBrakeTorque=800;
					maxHandbrakeTorque=2000;
					wheelHubMass=5;
					wheelHubRadius=0.125;
					class Suspension
					{
						stiffness=27500;
						compression=2100;
						damping=8000;
						travelMaxUp=0.16;
						travelMaxDown=0.16;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper="damper_1_2";
							inventorySlot="Hatchback_02_Wheel_1_2";
						};
						class Right: Right
						{
							animDamper="damper_2_2";
							inventorySlot="Hatchback_02_Wheel_2_2";
						};
					};
				};
			};
		};
	};
	class CivilianSedan: CarScript
	{
		class SimulationModule: SimulationModule
		{
			class Steering
			{
				maxSteeringAngle=35;
				increaseSpeed[]={0,50,10,40,30,30,60,15,100,7,120,4};
				decreaseSpeed[]={0,90,60,60,120,40};
				centeringSpeed[]={0,0,15,30,60,60,100,80};
			};
			class Throttle
			{
				reactionTime=0.34999999;
				defaultThrust=0.64999998;
				gentleThrust=0.55000001;
				turboCoef=6;
				gentleCoef=0.69999999;
			};
			class Brake
			{
				pressureBySpeed[]={0,0.43000001,10,0.38,20,0.34999999,40,0.25,50,0.27000001,80,0.28999999,100,0.31,140,0.38};
				reactionTime=0.2;
				driverless=0.1;
			};
			class Aerodynamics
			{
				frontalArea=2.2;
				dragCoefficient=0.44999999;
				downforceCoefficient=0.80000001;
				downforceOffset[]={0,0.40000001,-2.2};
			};
			drive="DRIVE_RWD";
			class Engine
			{
				torqueCurve[]={400,0,1000,185,1700,220,2500,230,4500,180,6000,0}; // torqueCurve[]={400,0,1000,157,1700,183,2500,187,4500,147,6000,0};
				inertia=0.44999999;
				frictionTorque=130;
				rollingFriction=2.5;
				viscousFriction=1;
				rpmIdle=900;
				rpmMin=1000;
				rpmClutch=1250;
				rpmRedline=4700;
			};
			class Clutch
			{
				maxTorqueTransfer=400;
				uncoupleTime=0.30000001;
				coupleTime=0.30000001;
			};
			class Gearbox
			{
				type="GEARBOX_MANUAL";
				reverse=3.51;
				ratios[]={3.5,2.26,1.45,1};
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxBrakeTorque=2000;
					wheelHubMass=10;
					wheelHubRadius=0.15000001;
					class Suspension
					{
						stiffness=60000;
						compression=2100;
						damping=5500;
						travelMaxUp=0.079000004;
						travelMaxDown=0.059999999;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper="damper_1_1";
							inventorySlot="CivSedanWheel_1_1";
						};
						class Right: Right
						{
							animDamper="damper_2_1";
							inventorySlot="CivSedanWheel_2_1";
						};
					};
				};
				class Rear: Rear
				{
					maxBrakeTorque=1600;
					maxHandbrakeTorque=2200;
					wheelHubMass=10;
					wheelHubRadius=0.15000001;
					class Differential
					{
						ratio=4.0999999;
						type="DIFFERENTIAL_OPEN";
					};
					class Suspension
					{
						stiffness=34000;
						compression=2000;
						damping=5100;
						travelMaxUp=0.086000003;
						travelMaxDown=0.133;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper="damper_1_2";
							inventorySlot="CivSedanWheel_1_2";
						};
						class Right: Right
						{
							animDamper="damper_2_2";
							inventorySlot="CivSedanWheel_2_2";
						};
					};
				};
			};
		};
	};
	class Sedan_02: CarScript
	{
		class SimulationModule: SimulationModule
		{
			class Steering
			{
				maxSteeringAngle=35;
				increaseSpeed[]={0,40,30,20,60,10,100,5};
				decreaseSpeed[]={0,90,60,70,100,50};
				centeringSpeed[]={0,0,15,35,60,65,100,90};
			};
			class Throttle
			{
				reactionTime=0.80000001;
				defaultThrust=0.75;
				gentleThrust=0.5;
				turboCoef=3.8;
				gentleCoef=0.5;
			};
			class Brake
			{
				pressureBySpeed[]={0,1,10,0.94999999,20,0.5,40,0.40000001,80,0.55000001,100,0.60000002,120,0.80000001};
				reactionTime=0.2;
				driverless=0.1;
			};
			class Aerodynamics
			{
				frontalArea=2;
				dragCoefficient=0.34999999;
			};
			drive="DRIVE_RWD";
			class Engine
			{
				torqueCurve[]={600,0,990,60,1800,90,3000,100,5000,85,7000,0}; // torqueCurve[]={600,0,990,50,1800,75,3000,82,5000,71,7000,0};
				inertia=0.2;
				frictionTorque=80;
				rollingFriction=2.5;
				viscousFriction=1;
				rpmIdle=900;
				rpmMin=1000;
				rpmClutch=1500;
				rpmRedline=5750;
			};
			class Clutch
			{
				maxTorqueTransfer=165;
				uncoupleTime=0.1;
				coupleTime=0.30000001;
			};
			class Gearbox
			{
				type="GEARBOX_MANUAL";
				reverse=3.27;
				ratios[]={3.8,2.1199999,1.41,0.95999998};
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxBrakeTorque=800;
					wheelHubMass=5;
					wheelHubRadius=0.17;
					class Suspension
					{
						stiffness=25000;
						compression=1600;
						damping=5200;
						travelMaxUp=0.07;
						travelMaxDown=0.079999998;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper="damper_1_1";
							inventorySlot="Sedan_02_Wheel_1_1";
						};
						class Right: Right
						{
							animDamper="damper_2_1";
							inventorySlot="Sedan_02_Wheel_2_1";
						};
					};
				};
				class Rear: Rear
				{
					maxBrakeTorque=600;
					maxHandbrakeTorque=1200;
					wheelHubMass=5;
					wheelHubRadius=0.17;
					class Differential
					{
						ratio=4.2199998;
						type="DIFFERENTIAL_OPEN";
					};
					class Suspension
					{
						stiffness=21500;
						compression=1800;
						damping=5600;
						travelMaxUp=0.1;
						travelMaxDown=0.1;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper="damper_1_2";
							inventorySlot="Sedan_02_Wheel_1_2";
						};
						class Right: Right
						{
							animDamper="damper_2_2";
							inventorySlot="Sedan_02_Wheel_2_2";
						};
					};
				};
			};
		};
	};
};
class cfgWeapons
{
	class Rifle_Base;
	class CZ61_Base: Rifle_Base
	{
		modes[]=
		{
			"FullAuto",
			"SemiAuto"
		};	
	}
	class MP5K_Base: Rifle_Base
	{
		modes[]=
		{
			"FullAuto",
			"SemiAuto",
			"Burst"
		};
	}
	class UMP45_Base: Rifle_Base
	{
		modes[]=
		{
			"FullAuto",
			"SemiAuto"
		};
	}
	class PP19_Base: Rifle_Base
	{
		modes[]=
		{
			"FullAuto",
			"SemiAuto"
		};
	}
	class VSS_Base: Rifle_Base
	{
		modes[]=
		{
			"FullAuto",
			"SemiAuto"
		};
	}
	class AKM_Base: Rifle_Base
	{
		modes[]=
		{
			"FullAuto",
			"SemiAuto"
		};
	}
	class AK74_Base: Rifle_Base
	{
		modes[]=
		{
			"FullAuto",
			"SemiAuto"
		};
	};
	class AK101_Base: Rifle_Base
	{
		modes[]=
		{
			"FullAuto",
			"SemiAuto"
		};
	};
	class Famas_Base: Rifle_Base
	{
		modes[]=
		{
			"FullAuto",
			"SemiAuto",
			"Burst"
		};		
	};
    class Aug_Base: Rifle_Base
	{
		modes[]=
		{
			"FullAuto",
			"SemiAuto",
			"Burst"
		};		
	};
	class M4A1_Base: Rifle_Base
	{
		modes[]=
		{
			"FullAuto",
			"SemiAuto"
		};
	};
};
class cfgslots
{
	class Slot_BattlegroundsLock
	{
		name="BattlegroundsLock";
		displayName="Faction Lock";
		ghostIcon="set:dayz_inventory image:combolock";
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyBaseBuilding;
	class ProxyBattlegroundsLock: ProxyBaseBuilding
	{
		model="\DZ\gear\camping\combination_lock_proxied.p3d";
		inventorySlot[]=
		{
			"BattlegroundsLock"
		};
		class AnimationSources
		{
			class AnimSourceHidden
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class Combination_Lock_Item: AnimSourceHidden
			{};
			class Combination_Lock_Attached: AnimSourceHidden
			{};
		};
	};
};