#pragma once
#include "main.h"

class Config {
public:
	void SaveConfig();
	void LoadConfig();
	
	int InGameFireKey = 191;
	
	float AdaptiveSmoothMultiplier = 1.0f;
	float RiflesAdaptiveSmoothMultiplier = 1.0f;
	float PistolsAdaptiveSmoothMultiplier = 1.0f;
	float SnipersAdaptiveSmoothMultiplier = 1.0f;
	float SMGsAdaptiveSmoothMultiplier = 1.0f;
	float ShotGunsAdaptiveSmoothMultiplier = 1.0f;
	float LMGsAdaptiveSmoothMultiplier = 1.0f;

	bool StreamMode = false;
	bool AspectRatio = false;
	bool GrenadePrediction = false;
	bool RecoilCrosshair = false;
	bool SkyBoxChanger = false;
	bool ViewModelFOVChanger = false;
	bool DisablePostProcessing = false;
	bool SniperCrosshair = false;
	bool RevealRank = false;
	bool ThirdPerson = false;
	bool RiflesAdaptiveSmooth = false;
	bool PistolsAdaptiveSmooth = false;
	bool SnipersAdaptiveSmooth = false;
	bool SMGsAdaptiveSmooth = false;
	bool ShotGunsAdaptiveSmooth = false;
	bool LMGsAdaptiveSmooth = false;
	bool spiralAim = false;
	float spiralAimX = 1.3f;
	float spiralAimY = 3.7f;
	bool faceitAim = false;
	bool SaveTargetAim = false;
	bool SprayFOV = false;
	bool bspParsing = false;
	bool skinchangerAutoUpdate = true;
	bool glowEspEnabled = true;
	bool aimbotEnabled = false;
	bool triggerbotEnabled = false;
	bool bunnyhopEnabled = false;
	bool skinchangerEnabled = false;
	bool fastReload = false;
	bool radar = false;
	bool panicMode = false;
	bool debugEnable = false;
	bool hitsound = false;

	bool needIndexes = true;
	
	struct keys {
		int aimbot_toggle;
		int aimbot_hold;
		int glowesp_toggle;
		int triggerbot_hold;
		int bunnyhop_hold;
		int chams_toggle;
		int nightmode_toggle;
		int radar_toggle;
		int skinchanger_toggle;
		int streamode_toggle;
		int thirdperson_toggle;
	}keys;

	struct aimbot {
		
		bool enable = false;
		int startbullet = 0;
		int endbullet = 0;
		float delay = 0.f;
		float fov = 0.f;
		float smooth = 0.f;
		int bone = 0;
		float rcsAmount = 0.f;
		float killdelay = 0.f;
		
	}aimbot[100];

	struct rcs {
		bool enable = false;
		float smooth = 0.f;
	}rcs;
	
	struct general_skin {
		char* knife = NULL;
		bool auto_update = false;
		bool knife_enable = false;
		int knife_skin_id = 569;
	}general_skin;

	struct skin {
		
		int id = 0;
		int statTrack = 0;
		
	}skin[100];

	struct skybox {
		char* name;
	}skybox;

	struct viewmodel {
		float fov;
	}viewmodel;
	
	struct chams {
		bool enabled;
		bool health_based;
		float brightness = 75.f;
		Color enemycolor;
		Color allycolor;
		Color c4color;
		Color c4playercolor;
		Color defusecolor;
		bool enemiesonly = false;
		bool modelambient = false;
		bool viewmodel_chams = false;
	}chams;

	struct aspect_ratio {
		int value = 0;
	}aspect_ratio;

	struct nightmode {
		bool enabled = false;
		float amount = 20.f;
	}nightmode;

	struct glowesp {
		bool noflicker;
		bool onlyVisible;
		bool health_based;
		Color color;
	}glowesp;
	
	struct triggerbot {
		bool fovBased;
		float fov = 2;
		float delay;
	}triggerbot;
	
	int knifeModel = 0;
	
private:	
	char* WeaponType(int ID);
	
	std::unordered_map<int, std::string> m_mapWeapons =
	{
		{ 1, xorstr("deagle").crypt_get() }, { 2, xorstr("dual_berettas").crypt_get() }, { 3, xorstr("five_seven").crypt_get() },
		{ 4, xorstr("glock").crypt_get()} , { 7, xorstr("ak47").crypt_get() }, { 8, xorstr("aug").crypt_get() },
		{ 9, xorstr("awp").crypt_get() }, { 10, xorstr("famas").crypt_get() }, { 11, xorstr("g3sg1").crypt_get() },
		{13, xorstr("galil").crypt_get()}, { 14, xorstr("m249").crypt_get()}, {16, xorstr("m4a4").crypt_get()},
		{17, xorstr("mac10").crypt_get()}, {19, xorstr("p90").crypt_get()}, {23, xorstr("mp5-sd").crypt_get()}, {24, xorstr("ump45").crypt_get()},
		{25, xorstr("xm1014").crypt_get()}, {26, xorstr("bizon").crypt_get()}, {27, xorstr("mag7").crypt_get()},
		{28, xorstr("negev").crypt_get()}, {29, xorstr("sawed_off").crypt_get()}, {30, xorstr("tec9").crypt_get()},
		{31, xorstr("zeus").crypt_get()}, {32, xorstr("p2000").crypt_get()}, {33, xorstr("mp7").crypt_get()},
		{34, xorstr("mp9").crypt_get()}, {35, xorstr("nova").crypt_get()}, {36, xorstr("p250").crypt_get()},
		{38, xorstr("scar-20").crypt_get()}, {39, xorstr("ssg553").crypt_get()}, {40, xorstr("scout").crypt_get()},
		{60, xorstr("m4a1s").crypt_get()}, {61, xorstr("usp").crypt_get()}, {63, xorstr("cz75").crypt_get()},
		{64, xorstr("r8").crypt_get()}
	};
};

extern Config* cfg;