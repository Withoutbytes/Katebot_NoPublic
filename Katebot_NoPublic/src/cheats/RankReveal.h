#ifndef _RANK_REVEAL_H_
#define _RANK_REVEAL_H_

#include "..\main.h"

class RankReveal {
public:
	void Start() {
		try {
			for (;;) {
				std::this_thread::sleep_for(std::chrono::milliseconds(40));

				if (!miscUtils->IsCSGOActiveWindow())
					continue;

				if (!csgo->IsInGame())
					continue;

				if (cfg->StreamMode)
					continue;

				if (!cfg->RevealRank)
					continue;

				if (!input_system::IsButtonDown(input_system::vktobc(VK_HOME))) continue;

				this->Terrorists.clear();
				this->CounterTerrorists.clear();

				csgo->ClientCMD(xorstr("clear").crypt_get());
				csgo->ClientCMD(xorstr("echo Rank Revealer").crypt_get());
				csgo->ClientCMD(xorstr("echo").crypt_get());

				for (auto e : EntityList) {
					if (!e.GetProfileInfo().xuid) continue;

					int entity_team_num = e.GetTeamNum();

					if (entity_team_num == 2)
						this->Terrorists.push_back(e);
					else if (entity_team_num == 3)
						this->CounterTerrorists.push_back(e);
				}

				csgo->ClientCMD(xorstr("echo Terrorists").crypt_get());
				//csgo->ClientCMD(xorstr("echo \").crypt_get()==============================\xorstr("").crypt_get());

				for (auto t : this->Terrorists)
				{
					std::ostringstream ss;

					ss << "echo \"" << t.GetProfileInfo().name << xorstr(" - [").crypt_get() <<
						this->Ranks[t.GetRank()] << xorstr("] - ").crypt_get() <<
						t.GetWins() << xorstr(" wins").crypt_get();

					csgo->ClientCMD(ss.str());
				}

				csgo->ClientCMD("echo");

				csgo->ClientCMD(xorstr("echo Counter-Terrorists").crypt_get());
				csgo->ClientCMD("echo \"==============================\"");

				for (auto ct : this->CounterTerrorists)
				{
					std::ostringstream ss;

					ss << "echo \"" << ct.GetProfileInfo().name << " - [" <<
						this->Ranks[ct.GetRank()] << "] - " <<
						ct.GetWins() << " wins";

					csgo->ClientCMD(ss.str());
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(1500));
			}
		}
		catch (...) {
			mem->debuglog(__FILE__);
		}
	}
private:
	std::vector<Entity> Terrorists;
	std::vector<Entity> CounterTerrorists;

	std::string Ranks[19] =
	{
		xorstr("Unranked").crypt_get(),
		xorstr("Silver I").crypt_get(),
		xorstr("Silver II").crypt_get(),
		xorstr("Silver III").crypt_get(),
		xorstr("Silver IV").crypt_get(),
		xorstr("Silver Elite").crypt_get(),
		xorstr("Silver Elite Master").crypt_get(),

		xorstr("Gold Nova I").crypt_get(),
		xorstr("Gold Nova II").crypt_get(),
		xorstr("Gold Nova III").crypt_get(),
		xorstr("Gold Nova Master").crypt_get(),
		xorstr("Master Guardian I").crypt_get(),
		xorstr("Master Guardian II").crypt_get(),

		xorstr("Master Guardian Elite").crypt_get(),
		xorstr("Distinguished Master Guardian").crypt_get(),
		xorstr("Legendary Eagle").crypt_get(),
		xorstr("Legendary Eagle Master").crypt_get(),
		xorstr("Supreme Master First Class").crypt_get(),
		xorstr("Global Elite").crypt_get()
	};
};

#endif