modded class SVD extends SVD_Base {};
class FAL extends FAL_Base {};
class M4A1 extends M4A1_Base {};
class SawedoffFAMAS extends Famas_Base {};
class AKM extends AKM_Base {};
modded class Aug extends Aug_Base {};
class M14 extends M14_Base {};

class MVS_Combat_Vest_Heavy_Black extends ModularVestSystem_Heavy {};
class MVS_Combat_Vest_Heavy_OD extends ModularVestSystem_Heavy {};
class MVS_Altyn_Black extends MVS_Altyn_Helmet_Base {};
class MVS_Altyn_OD extends MVS_Altyn_Helmet_Base {};

const static ref array<string> BGGuns = {"SVD","FAL","M4A1","SawedoffFAMAS","AKM","Aug","M14"};
const static ref array<string> BGVests = {"PlateCarrierVest_Winter","PlateCarrierVest_Green","PlateCarrierVest_Black", "MVS_Combat_Vest_Heavy_Black", "MVS_Combat_Vest_Heavy_OD"};
const static ref array<string> BGHelmets = {"BallisticHelmet_Winter", "BallisticHelmet_Desert", "BallisticHelmet_Woodland", "BallisticHelmet_BDU", "MVS_Altyn_Black", "MVS_Altyn_OD"};
const static ref array<string> BGBackpacks = {"CoyoteBag_Winter", "CoyoteBag_Brown", "CoyoteBag_Green", "AliceBag_Green", "AliceBag_Camo", "AliceBag_Black", "MVS_Rucksack_Black"};

class V1Rewards 
{
    static void AddRewardLoot(EntityAI crate) 
    {
        int i;

		int rndIndex;
		EntityAI obj;
		EntityAI attachment;
        int firstrandomIndex = Math.RandomInt(0, BGGuns.Count());
		obj = crate.GetInventory().CreateInInventory(BGGuns[firstrandomIndex]);

		switch (BGGuns[firstrandomIndex])
		{
			case "SVD":
				SVD svdInstance;
				if (SVD.CastTo(svdInstance, obj))
				{
					attachment = svdInstance.GetInventory().CreateAttachment("PSO1Optic");
					if (attachment) attachment.GetInventory().CreateAttachment("Battery9V");
					svdInstance.SpawnAttachedMagazine("Mag_SVD_10Rnd");
				}
				for (i = 0; i < 3; i++) crate.GetInventory().CreateInInventory("Mag_SVD_10Rnd");
					break;

			case "FAL":
				FAL falInstance;
				if (FAL.CastTo(falInstance, obj))
				{
					attachment = falInstance.GetInventory().CreateAttachment("ACOGOptic_6x");
					if (attachment) attachment.GetInventory().CreateAttachment("Battery9V");
					falInstance.SpawnAttachedMagazine("Mag_FAL_20Rnd");
					falInstance.GetInventory().CreateAttachment("Fal_OeBttstck");
				}
				for (i = 0; i < 2; i++) crate.GetInventory().CreateInInventory("Mag_FAL_20Rnd");
					break;

			case "M4A1":
				M4A1 m4a1Instance;
				if (M4A1.CastTo(m4a1Instance, obj))
				{
					attachment = m4a1Instance.GetInventory().CreateAttachment("ReflexOptic");
					if (attachment) attachment.GetInventory().CreateAttachment("Battery9V");
					m4a1Instance.SpawnAttachedMagazine("Mag_STANAG_60Rnd");
					m4a1Instance.GetInventory().CreateAttachment("M4_OEBttstck");
					m4a1Instance.GetInventory().CreateAttachment("M4_PlasticHndgrd");
				}
				for (i = 0; i < 1; i++) crate.GetInventory().CreateInInventory("Mag_STANAG_60Rnd");
					break;

			case "SawedoffFAMAS":
				SawedoffFAMAS famasInstance;
				if (SawedoffFAMAS.CastTo(famasInstance, obj))
				{
					famasInstance.SpawnAttachedMagazine("Mag_FAMAS_25Rnd");
				}
				for (i = 0; i < 4; i++) crate.GetInventory().CreateInInventory("Mag_FAMAS_25Rnd");
					break;

			case "AKM":
				AKM akmInstance;
				if (AKM.CastTo(akmInstance, obj))
				{
					attachment = akmInstance.GetInventory().CreateAttachment("KobraOptic");
					if (attachment) attachment.GetInventory().CreateAttachment("Battery9V");
					akmInstance.SpawnAttachedMagazine("Mag_AKM_Drum75Rnd");
					akmInstance.GetInventory().CreateAttachment("AK_WoodBttstck");
					akmInstance.GetInventory().CreateAttachment("AK_WoodHndgrd");
				}
				for (i = 0; i < 1; i++) crate.GetInventory().CreateInInventory("Mag_AKM_Drum75Rnd");
					break;

			case "Aug":
				Aug augInstance;
				if (Aug.CastTo(augInstance, obj))
				{
					attachment = augInstance.GetInventory().CreateAttachment("ReflexOptic");
					if (attachment) attachment.GetInventory().CreateAttachment("Battery9V");
					augInstance.SpawnAttachedMagazine("Mag_STANAG_30Rnd");
				}
				for (i = 0; i < 3; i++) crate.GetInventory().CreateInInventory("Mag_STANAG_30Rnd");
					break;

			case "M14":
				M14 m14Instance;
				if (M14.CastTo(m14Instance, obj))
				{
					attachment = m14Instance.GetInventory().CreateAttachment("ACOGOptic_6x");
					if (attachment) attachment.GetInventory().CreateAttachment("Battery9V");
					m14Instance.SpawnAttachedMagazine("Mag_M14_20Rnd");
				}
				for (i = 0; i < 2; i++) crate.GetInventory().CreateInInventory("Mag_M14_20Rnd");
					break;

			default:
				break;
		}

		int secondrandomIndex = Math.RandomInt(0, BGVests.Count());
		obj = crate.GetInventory().CreateAttachment(BGVests[secondrandomIndex]);

		switch (BGVests[secondrandomIndex])
		{
			case "PlateCarrierVest_Winter":
				PlateCarrierVest_Winter plateWinter;
				if (PlateCarrierVest_Winter.CastTo(plateWinter, obj))
					plateWinter.GetInventory().CreateAttachment("PlateCarrierPouches_Winter");
					break;

			case "PlateCarrierVest_Green":
				PlateCarrierVest_Green plateGreen;
				if (PlateCarrierVest_Green.CastTo(plateGreen, obj))
					plateGreen.GetInventory().CreateAttachment("PlateCarrierPouches_Green");
					break;

			case "PlateCarrierVest_Black":
				PlateCarrierVest_Black plateBlack;
				if (PlateCarrierVest_Black.CastTo(plateBlack, obj))
					plateBlack.GetInventory().CreateAttachment("PlateCarrierPouches_Black");
					break;

			case "MVS_Combat_Vest_Heavy_Black":
				MVS_Combat_Vest_Heavy_Black heavyBlack;
				if (MVS_Combat_Vest_Heavy_Black.CastTo(heavyBlack, obj))
					heavyBlack.GetInventory().CreateAttachment("MVS_Heavy_Pouch_Black");
					break;

			case "MVS_Combat_Vest_Heavy_OD":
				MVS_Combat_Vest_Heavy_OD heavyGreen;
				if (MVS_Combat_Vest_Heavy_OD.CastTo(heavyGreen, obj))
					heavyGreen.GetInventory().CreateAttachment("MVS_Heavy_Pouch_OD");
					break;

			default:
				break;
		}

		int thirdrandomIndex = Math.RandomInt(0, BGHelmets.Count());
		obj = crate.GetInventory().CreateAttachment(BGHelmets[thirdrandomIndex]);

		switch (BGHelmets[thirdrandomIndex])
		{
			case "MVS_Altyn_Black":
				MVS_Altyn_Black altynBlack;
				if (MVS_Altyn_Black.CastTo(altynBlack, obj))
					altynBlack.GetInventory().CreateAttachment("MVS_Altyn_Visor_Black");
					break;

			case "MVS_Altyn_OD":
				MVS_Altyn_OD altynGreen;
				if (MVS_Altyn_OD.CastTo(altynGreen, obj))
					altynGreen.GetInventory().CreateAttachment("MVS_Altyn_Visor_OD");
					break;

			default:
				break;
		}

		int fourthrandomIndex = Math.RandomInt(0, BGBackpacks.Count());
		obj = crate.GetInventory().CreateAttachment(BGBackpacks[fourthrandomIndex]);
	}
};