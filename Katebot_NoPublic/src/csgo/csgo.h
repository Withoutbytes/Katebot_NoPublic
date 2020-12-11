#pragma once

enum SIGNONSTATE
{
	SIGNONSTATE_NONE = 0,
	SIGNONSTATE_CHALLENGE = 1,
	SIGNONSTATE_CONNECTED = 2,
	SIGNONSTATE_NEW = 3,
	SIGNONSTATE_PRESPAWN = 4,
	SIGNONSTATE_SPAWN = 5,
	SIGNONSTATE_FULL = 6,
	SIGNONSTATE_CHANGELEVEL = 7
};

struct GlobalVarsBase
{
	float realtime;
	int framecount;
	float absoluteframetime;
	float absoluteframestarttimestddev;
	float curtime;
	float frametime;
	int maxClients;
	int tickcount;
	float interval_per_tick;
	float interpolation_amount;
};

class Engine
{
public:
	Engine();
	~Engine();

	GlobalVarsBase GlobalVars();

	void		ClientCMD(std::string cmd);

	int			GetLocalPlayer();
	int 		GetMaxObjects();
	int			GetMaxEntities();

	int 		GetGameType();
	
	void		GetViewAngles( Vector& viewangles );
	void		SetViewAngles( const Vector& viewangles );
	Vector		RetViewAngles();
	bool		IsConnected();
	bool		IsInGame();
	bool		IsInMyTeam(DWORD Entity);
	bool 		IsTeamSelected();
	bool 		IsDangerZone();

	void        SetViewAnglesSilent( const Vector& viewangles );
	void        SetViewAnglesPSilent( Vector& viewangles );
	
	std::string GetGameDirectory();
	const char* GetMapDirectory();
	
	void 		GlobalsSetup();
	float		GetCurTime();
	float		GetFrameTime();
	int			GetMaxClients();
	float		GetIntervalPerTick();
	float   	GetInterpolationAmount();

	int 		GetClassID(DWORD adr);
	char* 		GetClassNameFromPlayer(DWORD adr);
	bool 		IsClassIDAWeapon(int iClassID);
	bool		IsClassIDAGrenade(int iClassID);
	
	void		PAttack();
	void		MAttack();
	void		PressAttackKey();
	void		ReleaseAttackKey();
	void		ForceFullUpdate();
public:
	char		m_pMapName[ 32 ];
	char		m_pMapDirectory[ 255 ];
private:
	int m_chokedPackets = 0;
	bool m_sendpackets;
	DWORD_PTR m_dwGlobals;
};

extern Engine* csgo;