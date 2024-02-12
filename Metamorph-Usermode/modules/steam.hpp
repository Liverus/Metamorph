
#include "module.h"

#define REMOVE_STEAM_ACCOUNT

DEFINE_MODULE(Steam,
	{
		long int active_sid = 0;

		GetRegDWORD(HKEY_CURRENT_USER, XOR("Software\\Valve\\Steam\\ActiveProcess"), XOR("ActiveUser"), {
			active_sid = value;
		});

		if (active_sid == 0) {

			Error(XOR("You are not connected to Steam, aborting Steam Module"));

			return STATUS_UNSUCCESSFUL;
		}

		int count = 0;

		GetRegString(HKEY_CURRENT_USER, XOR("Software\\Valve\\Steam"), XOR("SteamPath"), {

			std::filesystem::path steam_path = value;

			auto userdata = steam_path / "userdata";

			auto userdata_iter = std::filesystem::directory_iterator(userdata);

			for (const auto& dirEntry : userdata_iter) {

				count++;

				auto sid = dirEntry.path().filename().u8string();

				auto profile = userdata / sid;
				auto config = profile / "config" / "localconfig.vdf";

				if (!std::filesystem::exists(config)) {
					Error(XOR("  Steam Account: %s has no config file"), sid);
					continue;
				}

				try
				{
					std::ifstream file(config);

					auto root = tyti::vdf::read(file);

					if (root.name != "UserLocalConfigStore") throw "Invalid Root";

					if (!root.childs.count("friends")) throw "Invalid Friends";

					auto friends = root.childs["friends"];

					if (!friends->attribs.count("PersonaName")) throw "Invalid PersonaName";

					auto persona = friends->attribs["PersonaName"];

					auto name = persona.c_str();

					long int sid_n = std::stol(sid);

					if (sid_n == active_sid) {
						Success(XOR("  Steam Account: %s (%s) is connected"), name, sid);
					} else {
						//if (std::filesystem::remove_all(profile) == -1) {
						//	Error(XOR("  Failed to remove Steam Account %d (%s)"), name, sid);
						//} else {
						//	Warning(XOR("  Steam Account: %s (%s) has been removed"), name, sid);
						//}

						Alert(XOR("  Steam Account: %s (%s) is disconnected"), name, sid);
					}
				}
				catch (const char* err)
				{
					Error(XOR("  Steam Account: %s has an invalid config file (%s)"), sid, err);
				}
			}
		});
		
		if (count == 0) {
			Warning(XOR("  No Steam Account found"));
		}
	}
)