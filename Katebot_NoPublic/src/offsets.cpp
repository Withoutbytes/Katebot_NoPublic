#include "main.h"
#include "csgo\netvars.h"

#define EXP(x) x, sizeof(x) - 1

Ofs* ofs = new Ofs();

std::string Ofs::toHex(DWORD offset) {
	std::stringstream ss;
	ss << std::hex << offset;
	std::string x = ss.str();
	for (auto& c : x) c = toupper(c);

	return x;
}

void Ofs::FindOffsets() {
	if (!c_netvars::get().init())
	{
		printf(xorstr("Can't initialize netvar manager!").crypt_get());
		return;
	}

	if (cfg->debugEnable)
		c_netvars::get().dump();

	static auto find_ptr = [](Module* mod, const char* sig, DWORD sig_add, DWORD off_add, bool sub_base = true, bool mode_read = true) -> DWORD
	{
		auto off = mem->FindPattern(mod, sig);
		auto sb = sub_base ? mod->GetImage() : 0;
		if (mode_read) off = mem->Read<DWORD>(off + sig_add);

		return (!off ? 0 : off + off_add - sb);
	};

	m_dwIndex = 0x64;

	m_dwClientState = find_ptr(engine, xorstr("A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0").crypt_get(), 0x1, 0); // up
	m_dwClientState_PlayerInfo = find_ptr(engine, xorstr("8B 89 ? ? ? ? 85 C9 0F 84 ? ? ? ? 8B 01").crypt_get(), 0x2, 0, false); // up
	m_dwViewAngles = find_ptr(engine, xorstr("F3 0F 11 80 ? ? ? ? D9 46 04 D9 05").crypt_get(), 0x4, 0, false); // up
	m_dwGameState = find_ptr(engine, xorstr("83 B8 ? ? ? ? ? 0F 94 C0 C3").crypt_get(), 0x2, 0, false); // unknown
	m_nDeltaTick = find_ptr(engine, xorstr("C7 87 ? ? ? ? ? ? ? ? FF 15 ? ? ? ? 83 C4 08").crypt_get(), 0x2, 0, false); // up
	m_dwLocalPlayer = find_ptr(client, xorstr("8D 34 85 ? ? ? ? 89 15 ? ? ? ? 8B 41 08 8B 48 04 83 F9 FF").crypt_get(), 0x3, 0x4); // up
	m_dwEntityList = find_ptr(client, xorstr("BB ? ? ? ? 83 FF 01 0F 8C ? ? ? ? 3B F8").crypt_get(), 0x1, 0); // up
	m_dwGlowObject = find_ptr(client, xorstr("A1 ? ? ? ? A8 01 75 4B").crypt_get(), 0x1, 0x4); // up
	m_dwRadarBase = find_ptr(client, xorstr("A1 ? ? ? ? 8B 0C B0 8B 01 FF 50 ? 46 3B 35 ? ? ? ? 7C EA 8B 0D").crypt_get(), 0x1, 0); // up
	m_dwForceJump = find_ptr(client, xorstr("8B 0D ? ? ? ? 8B D6 8B C1 83 CA 02").crypt_get(), 0x2, 0); // up
	m_dwForceBackward = find_ptr(client, xorstr("55 8B EC 51 53 8A 5D 08").crypt_get(), 287, 0); // up
	m_dwForceForward = find_ptr(client, "55 8B EC 51 53 8A 5D 08", 285, 0); // up
	m_dwForceLeft = find_ptr(client, "55 8B EC 51 53 8A 5D 08", 465, 0); // up
	m_dwForceRight = find_ptr(client, "55 8B EC 51 53 8A 5D 08", 512, 0); // up
	m_dwForceAttack = find_ptr(client, xorstr("89 0D ? ? ? ? 8B 0D ? ? ? ? 8B F2 8B C1 83 CE 04").crypt_get(), 0x2, 0); // up
	m_bDormant = find_ptr(client, xorstr("8A 81 ? ? ? ? C3 32 C0").crypt_get(), 0x2, 0x8, false); // up
	m_dwGlobalVars = find_ptr(engine, xorstr("68 ? ? ? ? 68 ? ? ? ? FF 50 08 85 C0").crypt_get(), 0x1, 0); // up
	m_dwPlayerResource = find_ptr(client, xorstr("8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7").crypt_get(), 0x2, 0); // up
	m_dwGameRulesProxy = find_ptr(client, xorstr("A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 80 B8 ? ? ? ? ? 74 7A").crypt_get(), 0x1, 0);
	m_dwClientCMD = find_ptr(engine, xorstr("55 8B EC 8B 0D ? ? ? ? 81 F9 ? ? ? ? 75 0C A1 ? ? ? ? 35 ? ? ? ? EB 05 8B 01 FF 50 34 50").crypt_get(), 0, 0, true, false);
	m_dwRankRevealAllFn = find_ptr(client, xorstr("55 8B EC 8B 0D ? ? ? ? 85 C9 75 28 A1 ? ? ? ? 68 ? ? ? ? 8B 08 8B 01 FF 50 04 85 C0 74 0B 8B C8 E8 ? ? ? ? 8B C8 EB 02 33 C9 89 0D ? ? ? ? 8B 45 08").crypt_get(), 0, 0, true, false);
	modelAmbientMin = find_ptr(engine, xorstr("F3 0F 10 0D ? ? ? ? F3 0F 11 4C 24 ? 8B 44 24 20 35 ? ? ? ? 89 44 24 0C").crypt_get(), 0x4, 0); // up

	m_hActiveWeapon = GET_NETVAR(xorstr("DT_BaseCombatCharacter").crypt_get(), xorstr("m_hActiveWeapon").crypt_get());
	m_totalRoundsPlayed = GET_NETVAR(xorstr("DT_CSGameRulesProxy").crypt_get(), xorstr("m_totalRoundsPlayed").crypt_get());
	m_iMatchStats_RoundResults = GET_NETVAR("DT_CSGameRulesProxy", xorstr("m_iMatchStats_RoundResults").crypt_get());
	m_bHasMatchStarted = GET_NETVAR("DT_CSGameRulesProxy", xorstr("m_bHasMatchStarted").crypt_get());
	m_iCompetitiveRanking = GET_NETVAR(xorstr("DT_CSPlayerResource").crypt_get(), xorstr("m_iCompetitiveRanking").crypt_get());
	m_iCompetitiveWins = GET_NETVAR("DT_CSPlayerResource", xorstr("m_iCompetitiveWins").crypt_get());
	m_bWarmupPeriod = GET_NETVAR("DT_CSGameRulesProxy", xorstr("m_bWarmupPeriod").crypt_get());
	m_iCrossHairID = GET_NETVAR(xorstr("DT_CSPlayer").crypt_get(), xorstr("m_bHasDefuser").crypt_get()) + 0x5C;
	m_iHealth = GET_NETVAR(xorstr("DT_BasePlayer").crypt_get(), xorstr("m_iHealth").crypt_get());
	m_iMaxHealth = GET_NETVAR(xorstr("DT_CHostage").crypt_get(), xorstr("m_iMaxHealth").crypt_get());
	m_iTeamNum = GET_NETVAR(xorstr("DT_BaseEntity").crypt_get(), xorstr("m_iTeamNum").crypt_get());
	m_nSurvivalTeam = GET_NETVAR("DT_CSPlayer", xorstr("m_nSurvivalTeam").crypt_get());
	m_flC4Blow = GET_NETVAR(xorstr("DT_PlantedC4").crypt_get(), xorstr("m_flC4Blow").crypt_get());
	m_iShotsFired = GET_NETVAR("DT_CSPlayer", xorstr("m_iShotsFired").crypt_get());
	m_totalHitsOnServer = GET_NETVAR("DT_CSPlayer", xorstr("m_totalHitsOnServer").crypt_get());
	m_fFlags = GET_NETVAR("DT_BasePlayer", xorstr("m_fFlags").crypt_get());
	m_lifeState = GET_NETVAR("DT_BasePlayer", xorstr("m_lifeState").crypt_get());
	m_bSpotted = GET_NETVAR("DT_BaseEntity", xorstr("m_bSpotted").crypt_get());
	m_bSpottedByMask = GET_NETVAR("DT_BaseEntity", xorstr("m_bSpottedByMask").crypt_get());
	m_bIsScoped = GET_NETVAR("DT_CSPlayer", xorstr("m_bIsScoped").crypt_get());
	m_zoomLevel = GET_NETVAR(xorstr("DT_WeaponCSBaseGun").crypt_get(), xorstr("m_zoomLevel").crypt_get());
	m_vecOrigin = GET_NETVAR("DT_BaseEntity", xorstr("m_vecOrigin").crypt_get());
	m_vecViewOffset = GET_NETVAR("DT_BasePlayer", xorstr("m_vecViewOffset[0]").crypt_get());
	m_angEyeAngles = GET_NETVAR("DT_CSPlayer", xorstr("m_angEyeAngles").crypt_get());
	m_vecVelocity = GET_NETVAR("DT_BasePlayer", xorstr("m_vecVelocity[0]").crypt_get());
	m_aimPunchAngle = GET_NETVAR("DT_CSPlayer", xorstr("m_aimPunchAngle").crypt_get());
	m_dwBoneMatrix = GET_NETVAR(xorstr("DT_BaseAnimating").crypt_get(), xorstr("m_nForceBone").crypt_get()) + 0x1C;
	m_flNextPrimaryAttack = GET_NETVAR(xorstr("DT_BaseCombatWeapon").crypt_get(), xorstr("m_flNextPrimaryAttack").crypt_get());
	m_SurvivalGameRuleDecisionTypes = GET_NETVAR("DT_CSGameRulesProxy", xorstr("m_SurvivalGameRuleDecisionTypes").crypt_get());
	m_nTickBase = GET_NETVAR("DT_BasePlayer", xorstr("m_nTickBase").crypt_get());
	m_iPlayerC4 = GET_NETVAR("DT_CSPlayerResource", xorstr("m_iPlayerC4").crypt_get());
	m_bIsDefusing = GET_NETVAR("DT_CSPlayer", xorstr("m_bIsDefusing").crypt_get());
	m_iItemDefinitionIndex = GET_NETVAR("DT_BaseCombatWeapon", xorstr("m_iItemDefinitionIndex").crypt_get());
	m_iWorldModelIndex = GET_NETVAR("DT_BaseCombatWeapon", xorstr("m_iWorldModelIndex").crypt_get());
	m_iEntityQuality = GET_NETVAR(xorstr("DT_BaseAttributableItem").crypt_get(), xorstr("m_iEntityQuality").crypt_get());
	m_iViewModelIndex = GET_NETVAR("DT_BaseCombatWeapon", xorstr("m_iViewModelIndex").crypt_get());
	m_nModelIndex = GET_NETVAR("DT_BaseCombatWeapon", xorstr("m_nModelIndex").crypt_get());
	m_hViewModel = GET_NETVAR("DT_CSPlayer", xorstr("m_hViewModel[0]").crypt_get());
	m_iAccountID = GET_NETVAR("DT_BaseCombatWeapon", xorstr("m_iAccountID").crypt_get());
	m_nFallbackPaintKit = GET_NETVAR("DT_BaseAttributableItem", xorstr("m_nFallbackPaintKit").crypt_get());
	m_OriginalOwnerXuidLow = GET_NETVAR("DT_BaseCombatWeapon", xorstr("m_OriginalOwnerXuidLow").crypt_get());
	m_iItemIDLow = GET_NETVAR("DT_BaseCombatWeapon", xorstr("m_iItemIDLow").crypt_get());
	m_iItemIDHigh = GET_NETVAR("DT_BaseAttributableItem", xorstr("m_iItemIDHigh").crypt_get());
	m_flFallbackWear = GET_NETVAR("DT_BaseAttributableItem", xorstr("m_flFallbackWear").crypt_get());
	m_hMyWeapons = GET_NETVAR("DT_BaseCombatCharacter", xorstr("m_hMyWeapons").crypt_get());
	m_iClip1 = GET_NETVAR("DT_WeaponCSBaseGun", xorstr("m_iClip1").crypt_get());

	m_hTonemapController = GET_NETVAR("DT_CSPlayer", xorstr("m_hTonemapController").crypt_get());
	m_bUseCustomAutoExposureMin = GET_NETVAR(xorstr("DT_EnvTonemapController").crypt_get(), xorstr("m_bUseCustomAutoExposureMin").crypt_get());
	m_bUseCustomAutoExposureMax = GET_NETVAR("DT_EnvTonemapController", xorstr("m_bUseCustomAutoExposureMax").crypt_get());

	m_flCustomBloomScale = GET_NETVAR("DT_EnvTonemapController", xorstr("m_flCustomBloomScale").crypt_get());
	m_flCustomAutoExposureMin = GET_NETVAR("DT_EnvTonemapController", xorstr("m_flCustomAutoExposureMin").crypt_get());
	m_flCustomAutoExposureMax = GET_NETVAR("DT_EnvTonemapController", xorstr("m_flCustomAutoExposureMax").crypt_get());

	m_bInReload = m_flNextPrimaryAttack + 0x6D;
	m_bReloadVisuallyComplete = GET_NETVAR("DT_WeaponCSBaseGun", xorstr("m_bReloadVisuallyComplete").crypt_get());


}

bool Ofs::CheckOffsets()
{
#define CHECK_OFFSET(n) \
	if (cfg->debugEnable) \
		printf(xorstr("%s = %X\n").crypt_get(), #n, n); \
	if (!n) { \
		result = false; }

	bool result = true;

	CHECK_OFFSET(m_dwIndex);

	CHECK_OFFSET(m_dwClientState);
	CHECK_OFFSET(m_dwViewAngles);
	CHECK_OFFSET(m_dwGameState);
	CHECK_OFFSET(m_nDeltaTick);
	CHECK_OFFSET(m_dwLocalPlayer);
	CHECK_OFFSET(m_dwEntityList);
	CHECK_OFFSET(m_dwGlowObject);
	CHECK_OFFSET(m_dwForceJump);
	CHECK_OFFSET(m_dwForceAttack);
	CHECK_OFFSET(m_bDormant);
	CHECK_OFFSET(m_dwGlobalVars);
	CHECK_OFFSET(m_dwPlayerResource);

	CHECK_OFFSET(m_hActiveWeapon);
	CHECK_OFFSET(m_iCrossHairID);
	CHECK_OFFSET(m_iHealth);
	CHECK_OFFSET(m_iTeamNum);
	CHECK_OFFSET(m_iShotsFired);
	CHECK_OFFSET(m_fFlags);
	CHECK_OFFSET(m_lifeState);
	CHECK_OFFSET(m_bSpottedByMask);
	CHECK_OFFSET(m_vecOrigin);
	CHECK_OFFSET(m_vecViewOffset);
	CHECK_OFFSET(m_angEyeAngles);
	CHECK_OFFSET(m_vecVelocity);
	CHECK_OFFSET(m_aimPunchAngle);
	CHECK_OFFSET(m_dwBoneMatrix);
	CHECK_OFFSET(m_flNextPrimaryAttack);
	CHECK_OFFSET(m_nTickBase);
	CHECK_OFFSET(m_iPlayerC4);
	CHECK_OFFSET(m_bIsDefusing);
	CHECK_OFFSET(m_iItemDefinitionIndex);
	CHECK_OFFSET(m_iWorldModelIndex);
	CHECK_OFFSET(m_nModelIndex);
	CHECK_OFFSET(m_hViewModel);
	CHECK_OFFSET(m_iAccountID);
	CHECK_OFFSET(m_nFallbackPaintKit);
	CHECK_OFFSET(m_OriginalOwnerXuidLow);
	CHECK_OFFSET(m_iItemIDLow);
	CHECK_OFFSET(m_hMyWeapons);

	return result;
}

void Ofs::getOffsets() {
	FindOffsets();

	if (!CheckOffsets())
	{
		printf(xorstr("Offsets outdated!\n").crypt_get());
		std::system(xorstr("pause").crypt_get());
		exit(1);
	}
	//std::system("pause");
	Sleep(1000);

	m_dwClientState = mem->Read<DWORD_PTR>(engine->GetImage() + m_dwClientState);
}
