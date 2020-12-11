#ifndef _FAST_RELOAD_H_
#define _FAST_RELOAD_H_

#include "..\main.h"

class FastReload {
public:
	void Start() {
		try {
			for (;;) {
				std::this_thread::sleep_for(std::chrono::milliseconds(30));

				if (!miscUtils->IsCSGOActiveWindow())
					continue;

				if (!cfg->fastReload)
					continue;

				if (!csgo->IsInGame())
					continue;

				if (!MouseEnable()) continue;

				int m_bReloadVisuallyComplete = mem->Read<int>(LocalEntity.GetActiveWeaponBase() + ofs->m_bReloadVisuallyComplete), ammo = LocalEntity.GetActiveWeaponAmmo();

				if (mem->Read<bool>(LocalEntity.GetActiveWeaponBase() + ofs->m_bInReload) &&
					(m_bReloadVisuallyComplete == 1 || m_bReloadVisuallyComplete == 257))
				{
					this->CMD_Reload();
				}
			}
		}
		catch (...) {
			mem->debuglog(__FILE__);
		}
	}

	int MouseEnable() {
		static auto cl_mouseenable = cvar::find(xorstr("cl_mouseenable").crypt_get());

		return cl_mouseenable.GetInt();
	}

	void Reload() {
		keybd_event(VkKeyScan('Q'), 0x10, 0, 0);
		Sleep(10);
		keybd_event(VkKeyScan('Q'), 0x10, KEYEVENTF_KEYUP, 0);
		Sleep(50);
		keybd_event(VkKeyScan('Q'), 0x10, 0, 0);
		Sleep(10);
		keybd_event(VkKeyScan('Q'), 0x10, KEYEVENTF_KEYUP, 0);
	}

	void CMD_Reload() {
		csgo->ClientCMD(xorstr("invprev").crypt_get());
		Sleep(20);
		csgo->ClientCMD(xorstr("invnext").crypt_get());
	}
};

#endif