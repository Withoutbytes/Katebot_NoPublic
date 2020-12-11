#include "main.h"

Config* cfg = new Config();



void Config::SaveConfig()
{
	for (auto& map_data : m_mapWeapons)
	{
		char* weaponName = new char[map_data.second.size() + 1];
		std::copy(map_data.second.begin(), map_data.second.end(), weaponName);
		weaponName[map_data.second.size()] = '\0';

		char* weapontype = WeaponType(map_data.first);

		g_pFiles->WriteBoolParam(weapontype, weaponName, xorstr(".aimbot.enable").crypt_get(), aimbot[map_data.first].enable);
		g_pFiles->WriteIntParam(weapontype, weaponName, xorstr(".aimbot.start").crypt_get(), aimbot[map_data.first].startbullet);
		g_pFiles->WriteIntParam(weapontype, weaponName, xorstr(".aimbot.end").crypt_get(), aimbot[map_data.first].endbullet);
		g_pFiles->WriteFloatParam(weapontype, weaponName, xorstr(".aimbot.delay").crypt_get(), aimbot[map_data.first].delay);
		g_pFiles->WriteFloatParam(weapontype, weaponName, xorstr(".aimbot.fov").crypt_get(), aimbot[map_data.first].fov);
		g_pFiles->WriteFloatParam(weapontype, weaponName, xorstr(".aimbot.smooth").crypt_get(), aimbot[map_data.first].smooth);
		g_pFiles->WriteIntParam(weapontype, weaponName, xorstr(".aimbot.bone").crypt_get(), aimbot[map_data.first].bone);
		g_pFiles->WriteFloatParam(weapontype, weaponName, xorstr(".aimbot.rcsAmount").crypt_get(), aimbot[map_data.first].rcsAmount);
		g_pFiles->WriteFloatParam(weapontype, weaponName, xorstr(".aimbot.killDelay").crypt_get(), aimbot[map_data.first].killdelay);

		delete[] weaponName;
	}
}

void Config::LoadConfig()
{
	keys.aimbot_toggle = g_pFiles->ReadInt(xorstr("Keys").crypt_get(), xorstr("Aimbot.Toggle").crypt_get());
	keys.aimbot_hold = g_pFiles->ReadInt("Keys", xorstr("Aimbot.Hold").crypt_get());
	keys.glowesp_toggle = g_pFiles->ReadInt("Keys", xorstr("GlowESP.Toggle").crypt_get());
	keys.triggerbot_hold = g_pFiles->ReadInt("Keys", xorstr("Triggerbot.Hold").crypt_get());
	keys.bunnyhop_hold = g_pFiles->ReadInt("Keys", xorstr("Bunnyhop.Hold").crypt_get());
	keys.skinchanger_toggle = g_pFiles->ReadInt("Keys", xorstr("Skinchanger.Toggle").crypt_get());
	keys.chams_toggle = g_pFiles->ReadInt("Keys", xorstr("Chams.Toggle").crypt_get());
	keys.nightmode_toggle = g_pFiles->ReadInt("Keys", xorstr("NightMode.Toggle").crypt_get());
	keys.radar_toggle = g_pFiles->ReadInt("Keys", xorstr("Radar.Toggle").crypt_get());
	keys.streamode_toggle = g_pFiles->ReadInt("Keys", xorstr("StreamMode.Toggle").crypt_get());
	//keys.panic_mode = g_pFiles->ReadInt("Keys",xorstr("PANIC_MODE").crypt_get());

	debugEnable = g_pFiles->ReadBool(xorstr("Features").crypt_get(), xorstr("Debug.Enable").crypt_get());
	StreamMode = g_pFiles->ReadBool("Features", xorstr("StreamMode.Enable").crypt_get());
	AspectRatio = g_pFiles->ReadBool("Features", xorstr("AspectRatio.Enable").crypt_get());
	GrenadePrediction = g_pFiles->ReadBool("Features", xorstr("GrenadePrediction.Enable").crypt_get());
	RecoilCrosshair = g_pFiles->ReadBool("Features", xorstr("RecoilCrosshair.Enable").crypt_get());
	ViewModelFOVChanger = g_pFiles->ReadBool("Features", xorstr("ViewModelFOV.Enable").crypt_get());
	DisablePostProcessing = g_pFiles->ReadBool("Features", xorstr("DisablePostProcessing.Enable").crypt_get());
	SniperCrosshair = g_pFiles->ReadBool("Features", xorstr("SniperCrosshair.Enable").crypt_get());
	RevealRank = g_pFiles->ReadBool("Features", xorstr("RevealRank.Enable").crypt_get());
	ThirdPerson = g_pFiles->ReadBool("Features", xorstr("ThirdPerson.Enable").crypt_get());
	hitsound = g_pFiles->ReadBool("Features", xorstr("HitSound.Enable").crypt_get());
	glowEspEnabled = g_pFiles->ReadBool("Features", xorstr("GlowESP.Enable").crypt_get());
	aimbotEnabled = g_pFiles->ReadBool("Features", xorstr("Aimbot.Enable").crypt_get());
	triggerbotEnabled = g_pFiles->ReadBool("Features", xorstr("Triggerbot.Enable").crypt_get());
	bunnyhopEnabled = g_pFiles->ReadBool("Features", xorstr("Bunnyhop.Enable").crypt_get());
	skinchangerEnabled = g_pFiles->ReadBool("Features", xorstr("Skinchanger.Enable").crypt_get());
	SkyBoxChanger = g_pFiles->ReadBool("Features", xorstr("SkyBoxChanger.Enable").crypt_get());
	chams.enabled = g_pFiles->ReadBool("Features", xorstr("Chams.Enable").crypt_get());
	radar = g_pFiles->ReadBool("Features", xorstr("Radar.Enable").crypt_get());
	nightmode.enabled = g_pFiles->ReadBool("Features", xorstr("NightMode.Enable").crypt_get());
	fastReload = g_pFiles->ReadBool("Features", xorstr("FastReload.Enable").crypt_get());
	hitsound = g_pFiles->ReadBool("Features", xorstr("Hitsound.Enable").crypt_get());
	rcs.enable = g_pFiles->ReadBool("Features", xorstr("RCS.Enable").crypt_get());

	RiflesAdaptiveSmoothMultiplier = g_pFiles->ReadFloat(xorstr("Aimbot").crypt_get(), xorstr("AdaptiveSmooth.Rifles.Multiplier").crypt_get());
	PistolsAdaptiveSmoothMultiplier = g_pFiles->ReadFloat("Aimbot", xorstr("AdaptiveSmooth.Pistol.Multiplier").crypt_get());
	SnipersAdaptiveSmoothMultiplier = g_pFiles->ReadFloat("Aimbot", xorstr("AdaptiveSmooth.Sniper.Multiplier").crypt_get());
	SMGsAdaptiveSmoothMultiplier = g_pFiles->ReadFloat("Aimbot", xorstr("AdaptiveSmooth.SMG.Multiplier").crypt_get());
	ShotGunsAdaptiveSmoothMultiplier = g_pFiles->ReadFloat("Aimbot", xorstr("AdaptiveSmooth.Shotgun.Multiplier").crypt_get());
	LMGsAdaptiveSmoothMultiplier = g_pFiles->ReadFloat("Aimbot", xorstr("AdaptiveSmooth.LMG.Multiplier").crypt_get());

	RiflesAdaptiveSmooth = g_pFiles->ReadBool("Aimbot", xorstr("AdaptiveSmooth.Rifles").crypt_get());
	PistolsAdaptiveSmooth = g_pFiles->ReadBool("Aimbot", xorstr("AdaptiveSmooth.Pistol").crypt_get());
	SnipersAdaptiveSmooth = g_pFiles->ReadBool("Aimbot", xorstr("AdaptiveSmooth.Sniper").crypt_get());
	SMGsAdaptiveSmooth = g_pFiles->ReadBool("Aimbot", xorstr("AdaptiveSmooth.SMG").crypt_get());
	ShotGunsAdaptiveSmooth = g_pFiles->ReadBool("Aimbot", xorstr("AdaptiveSmooth.Shotgun").crypt_get());
	LMGsAdaptiveSmooth = g_pFiles->ReadBool("Aimbot", xorstr("AdaptiveSmooth.LMG").crypt_get());

	spiralAim = g_pFiles->ReadBool("Aimbot", xorstr("Spiral.Enable").crypt_get());
	spiralAimX = g_pFiles->ReadFloat("Aimbot", xorstr("Spiral.X").crypt_get());
	spiralAimY = g_pFiles->ReadFloat("Aimbot", xorstr("Spiral.Y").crypt_get());
	SaveTargetAim = g_pFiles->ReadBool("Aimbot", xorstr("SaveTarget.Enable").crypt_get());
	faceitAim = g_pFiles->ReadBool("Aimbot", xorstr("FaceitMode.Enable").crypt_get());
	SprayFOV = g_pFiles->ReadBool("Aimbot", xorstr("SprayFOV.Enable").crypt_get());

	bspParsing = g_pFiles->ReadBool(xorstr("BSPParsing").crypt_get(), xorstr("Enable").crypt_get());

	aspect_ratio.value = g_pFiles->ReadFloat(xorstr("AspectRatio").crypt_get(), xorstr("Value").crypt_get());

	viewmodel.fov = g_pFiles->ReadFloat(xorstr("ViewModel").crypt_get(), xorstr("Fov").crypt_get());

	chams.modelambient = g_pFiles->ReadBool(xorstr("Chams").crypt_get(), xorstr("ModelAmbient.Enable").crypt_get());
	chams.health_based = g_pFiles->ReadBool("Chams", xorstr("HealthBased.Enable").crypt_get());
	chams.viewmodel_chams = g_pFiles->ReadBool("Chams", xorstr("ViewModel.Enable").crypt_get());
	chams.brightness = g_pFiles->ReadFloat("Chams", xorstr("Brightness").crypt_get());
	chams.enemiesonly = g_pFiles->ReadBool("Chams", xorstr("EnemiesOnly.Enable").crypt_get());
	chams.enemycolor.r = g_pFiles->ReadInt("Chams", xorstr("EnemyColor.Red").crypt_get());
	chams.enemycolor.g = g_pFiles->ReadInt("Chams", xorstr("EnemyColor.Green").crypt_get());
	chams.enemycolor.b = g_pFiles->ReadInt("Chams", xorstr("EnemyColor.Blue").crypt_get());
	chams.enemycolor.a = g_pFiles->ReadInt("Chams", xorstr("EnemyColor.Alpha").crypt_get());
	chams.allycolor.r = g_pFiles->ReadInt("Chams", xorstr("AllyColor.Red").crypt_get());
	chams.allycolor.g = g_pFiles->ReadInt("Chams", xorstr("AllyColor.Green").crypt_get());
	chams.allycolor.b = g_pFiles->ReadInt("Chams", xorstr("AllyColor.Blue").crypt_get());
	chams.allycolor.a = g_pFiles->ReadInt("Chams", xorstr("AllyColor.Alpha").crypt_get());
	chams.c4color.r = g_pFiles->ReadInt("Chams", xorstr("C4Bomb.Red").crypt_get());
	chams.c4color.g = g_pFiles->ReadInt("Chams", xorstr("C4Bomb.Green").crypt_get());
	chams.c4color.b = g_pFiles->ReadInt("Chams", xorstr("C4Bomb.Blue").crypt_get());
	chams.c4color.a = g_pFiles->ReadInt("Chams", xorstr("C4Bomb.Alpha").crypt_get());
	chams.c4playercolor.r = g_pFiles->ReadInt("Chams", xorstr("C4Player.Red").crypt_get());
	chams.c4playercolor.g = g_pFiles->ReadInt("Chams", xorstr("C4Player.Green").crypt_get());
	chams.c4playercolor.b = g_pFiles->ReadInt("Chams", xorstr("C4Player.Blue").crypt_get());
	chams.c4playercolor.a = g_pFiles->ReadInt("Chams", xorstr("C4Player.Alpha").crypt_get());
	chams.defusecolor.r = g_pFiles->ReadInt("Chams", xorstr("DefusePlayer.Red").crypt_get());
	chams.defusecolor.g = g_pFiles->ReadInt("Chams", xorstr("DefusePlayer.Green").crypt_get());
	chams.defusecolor.b = g_pFiles->ReadInt("Chams", xorstr("DefusePlayer.Blue").crypt_get());
	chams.defusecolor.a = g_pFiles->ReadInt("Chams", xorstr("DefusePlayer.Alpha").crypt_get());

	rcs.smooth = g_pFiles->ReadFloat(xorstr("Settings").crypt_get(), xorstr("RCS.smooth").crypt_get());
	general_skin.knife = g_pFiles->ReadChar(xorstr("SkinChanger").crypt_get(), xorstr("Knife").crypt_get());
	general_skin.auto_update = g_pFiles->ReadBool("SkinChanger", xorstr("AutoUpdate.Enable").crypt_get());
	general_skin.knife_enable = g_pFiles->ReadBool("SkinChanger", xorstr("Knife.Enable").crypt_get());
	general_skin.knife_skin_id = g_pFiles->ReadInt("SkinChanger", xorstr("Knife.SkinID").crypt_get());

	nightmode.amount = g_pFiles->ReadFloat(xorstr("NightMode").crypt_get(), xorstr("Amount").crypt_get());

	skinchangerAutoUpdate = g_pFiles->ReadBool("Settings", xorstr("Skinchanger.AutoUpdate").crypt_get());

	skybox.name = g_pFiles->ReadChar(xorstr("Skybox").crypt_get(), xorstr("Name").crypt_get());

	triggerbot.fovBased = g_pFiles->ReadBool(xorstr("Triggerbot").crypt_get(), xorstr("FovBased.Enable").crypt_get());
	triggerbot.fov = g_pFiles->ReadFloat("Triggerbot", "Fov");
	triggerbot.delay = g_pFiles->ReadFloat("Triggerbot", xorstr("Delay").crypt_get());

	glowesp.noflicker = g_pFiles->ReadBool(xorstr("GlowESP").crypt_get(), xorstr("NoFlicker.Enable").crypt_get());
	glowesp.onlyVisible = g_pFiles->ReadBool("GlowESP", xorstr("OnlyVisible.Enable").crypt_get());
	glowesp.health_based = g_pFiles->ReadBool("GlowESP", "HealthBased.Enable");
	glowesp.color.r = g_pFiles->ReadInt("GlowESP", xorstr("Color.Red").crypt_get());
	glowesp.color.g = g_pFiles->ReadInt("GlowESP", xorstr("Color.Green").crypt_get());
	glowesp.color.b = g_pFiles->ReadInt("GlowESP", xorstr("Color.Blue").crypt_get());
	glowesp.color.a = g_pFiles->ReadInt("GlowESP", xorstr("Color.Alpha").crypt_get());

	knifeModel = g_pFiles->ReadInt("Settings", xorstr("KnifeChanger.Model(0-10)").crypt_get());

	for (auto& map_data : m_mapWeapons)
	{
		char* weaponName = new char[map_data.second.size() + 1];
		std::copy(map_data.second.begin(), map_data.second.end(), weaponName);
		weaponName[map_data.second.size()] = '\0';

		char* weapontype = WeaponType(map_data.first);

		aimbot[map_data.first].enable = g_pFiles->ReadBoolParam(weapontype, weaponName, ".aimbot.enable");
		aimbot[map_data.first].startbullet = g_pFiles->ReadIntParam(weapontype, weaponName, ".aimbot.start");
		aimbot[map_data.first].endbullet = g_pFiles->ReadIntParam(weapontype, weaponName, ".aimbot.end");
		aimbot[map_data.first].delay = g_pFiles->ReadFloatParam(weapontype, weaponName, ".aimbot.delay");
		aimbot[map_data.first].fov = g_pFiles->ReadFloatParam(weapontype, weaponName, ".aimbot.fov");
		aimbot[map_data.first].smooth = g_pFiles->ReadFloatParam(weapontype, weaponName, ".aimbot.smooth");
		aimbot[map_data.first].bone = g_pFiles->ReadIntParam(weapontype, weaponName, ".aimbot.bone");
		aimbot[map_data.first].rcsAmount = g_pFiles->ReadFloatParam(weapontype, weaponName, ".aimbot.rcsAmount");
		aimbot[map_data.first].killdelay = g_pFiles->ReadFloatParam(weapontype, weaponName, ".aimbot.killDelay");
		skin[map_data.first].id = g_pFiles->ReadIntParam(weapontype, weaponName, xorstr(".skin.id").crypt_get());
		skin[map_data.first].statTrack = g_pFiles->ReadIntParam(weapontype, weaponName, xorstr(".skin.statTrack").crypt_get());

		delete[] weaponName;
	}

	needIndexes = true;
}

char* Config::WeaponType(int ID)
{
	switch (ID)
	{
	case WEAPON_NEGEV:
	case WEAPON_M249:
		return xorstr("LMG").crypt_get();

	case WEAPON_AWP:
	case WEAPON_G3SG1:
	case WEAPON_SCAR20:
	case WEAPON_SSG08:
		return xorstr("Snipers").crypt_get();

	case WEAPON_XM1014:
	case WEAPON_MAG7:
	case WEAPON_NOVA:
	case WEAPON_SAWEDOFF:
		return xorstr("Shotguns").crypt_get();

	case WEAPON_MAC10:
	case WEAPON_P90:
	case WEAPON_UMP45:
	case WEAPON_BIZON:
	case WEAPON_MP7:
	case WEAPON_MP9:
	case WEAPON_MP5_SD:
		return xorstr("SMGs").crypt_get();

	case WEAPON_DEAGLE:
	case WEAPON_ELITE:
	case WEAPON_FIVESEVEN:
	case WEAPON_USP_SILENCER:
	case WEAPON_GLOCK:
	case WEAPON_TEC9:
	case WEAPON_HKP2000:
	case WEAPON_P250:
	case WEAPON_CZ75A:
	case WEAPON_REVOLVER:
	case WEAPON_TASER:
		return xorstr("Pistols").crypt_get();

	case WEAPON_AK47:
	case WEAPON_AUG:
	case WEAPON_FAMAS:
	case WEAPON_M4A1:
	case WEAPON_M4A1_SILENCER:
	case WEAPON_GALILAR:
	case WEAPON_SG556:
		return xorstr("Rifles").crypt_get();

	default:
		return xorstr("null").crypt_get();
	}
}