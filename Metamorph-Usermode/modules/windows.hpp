
#include "spoofer.h"

#include <ctime>

struct RawSMBIOSData
{
	BYTE    Used20CallingMethod;
	BYTE    SMBIOSMajorVersion;
	BYTE    SMBIOSMinorVersion;
	BYTE    DmiRevision;
	DWORD    Length;
	BYTE    SMBIOSTableData[];
};

void GenerateComputerName(char* input) {

	const char* Alphabet = XOR("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
	
	memcpy(input, XOR("DESKTOP-"), 8);

	for (int i = 8; i < 16; ++i) {
		input[i] = Alphabet[rand() % sizeof(Alphabet)];
	}

	input[15] = '\0';
}

void GenerateMachineGUID(char* input) {
	UUID uuid = { 0 };

	UuidCreate(&uuid);

	RPC_CSTR szUuid = NULL;
	if (UuidToStringA(&uuid, &szUuid) == RPC_S_OK)
	{
		char* str = (char*)szUuid;
		memcpy(input, str, strlen(str) + 1);
		RpcStringFreeA(&szUuid);
	}
}

void GenerateProductID(char* input) {

	const char* Alphabet = XOR("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

	int nNumbers[3];

	for (int i = 0; i < 3; i++)
	{
		nNumbers[i] = (rand() % 99999);
	}

	char sString[5];

	for (int i = 0; i < 5; i++)
	{
		sString[i] = Alphabet[rand() % sizeof(Alphabet)];
	}

	sprintf_s(input, 28, XOR("%05d-%05d-%05d-%05s"), nNumbers[0], nNumbers[1], nNumbers[2], sString);
}

void GenerateDigitalProductID(char* input) {

	for (int i = 0; i < 166; i++)
	{
		input[i] = rand() % 0x100;
	}
}

void GenerateInstallDate(time_t* input) {
	time_t start = 1451653200;
	time_t now = time(NULL);

	time_t new_time = start + rand() / double(RAND_MAX) * (now-start);

	*input = new_time;
}

DEFINE_MODULE(Windows,
	{
		// ProductId
		char ProductId[28];
		GenerateProductID(ProductId);
		SetRegString(HKEY_LOCAL_MACHINE, XOR("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), XOR("ProductId"), ProductId);

		Success(XOR("  New ProductID: %s"), ProductId);

		// DigitalProductId
		char DigitalProductId[166];
		GenerateDigitalProductID(DigitalProductId);
		SetRegBinary(HKEY_LOCAL_MACHINE, XOR("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), XOR("DigitalProductId"), DigitalProductId, sizeof(DigitalProductId));

		auto DigitalProductIdHash = simple_hash(DigitalProductId, sizeof(DigitalProductId));

		Success(XOR("  New DigitalProductID: %llu"), DigitalProductIdHash);

		// Machine GUID
		char MachineGUID[36];
		GenerateMachineGUID(MachineGUID);
		SetRegString(HKEY_LOCAL_MACHINE, XOR("SOFTWARE\\Microsoft\\Cryptography"), XOR("MachineGuid"), MachineGUID);

		Success(XOR("  New MachineGUID: %s"), MachineGUID);

		// ComputerName
		char ComputerName[15];
		GenerateComputerName(ComputerName);
		SetRegString(HKEY_LOCAL_MACHINE, XOR("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters"), XOR("Hostname"), ComputerName);
		SetRegString(HKEY_LOCAL_MACHINE, XOR("SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ActiveComputerName"), XOR("ComputerName"), ComputerName);
		SetRegString(HKEY_LOCAL_MACHINE, XOR("SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName"), XOR("ComputerName"), ComputerName);

		Success(XOR("  New ComputerName: %s"), ComputerName);

		// Wallpaper
		const char* Wallpaper = XOR("C:\\WINDOWS\\web\\wallpaper\\Windows\\img19.jpg");
		SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (LPVOID)Wallpaper, SPIF_UPDATEINIFILE);

		Success(XOR("  New Wallpaper: %s"), Wallpaper);

		// Install date
		time_t InstallDate = 0;
		GenerateInstallDate(&InstallDate);
		SetRegDWORD(HKEY_LOCAL_MACHINE, XOR("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), XOR("InstallDate"), &InstallDate);

		char InstallDateFormat[90];
		struct tm timeinfo;
		localtime_s(&timeinfo, &InstallDate);
		strftime(InstallDateFormat, sizeof(InstallDateFormat), XOR("%Y-%m-%d %H:%M:%S"), &timeinfo);

		Success(XOR("  New InstallDate: %s"), InstallDateFormat);

		DWORD smBiosDataSize = 0;
		RawSMBIOSData* smBiosData = NULL;
		DWORD bytesWritten = 0;

		smBiosDataSize = GetSystemFirmwareTable('RSMB', 0, NULL, 0);

		smBiosData = (RawSMBIOSData*)HeapAlloc(GetProcessHeap(), 0, smBiosDataSize);
		
		if (!smBiosData) {
			Error(XOR("  Couldn't allocate firmware buffer"));
		}

		bytesWritten = GetSystemFirmwareTable('RSMB', 0, smBiosData, smBiosDataSize);

		if (bytesWritten != smBiosDataSize) {
			Error(XOR("  Couldn't access firmware table"));
		}

		SetRegBinary(HKEY_LOCAL_MACHINE, XOR("SYSTEM\\CurrentControlSet\\Services\\mssmbios\\Data"), XOR("SMBiosData"), smBiosData, smBiosDataSize);

		Success(XOR("  Synced SMBIOS Table"));
	}
)