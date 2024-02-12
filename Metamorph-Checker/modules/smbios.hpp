
#include "module.h"

struct RawSMBIOSData
{
    BYTE    Used20CallingMethod;
    BYTE    SMBIOSMajorVersion;
    BYTE    SMBIOSMinorVersion;
    BYTE    DmiRevision;
    DWORD   Length;
    BYTE    SMBIOSTableData[];
};

DEFINE_MODULE(Smbios,
    {
        DWORD error = ERROR_SUCCESS;
        DWORD smBiosDataSize = 0;
        RawSMBIOSData* smBiosData = NULL;
        DWORD bytesWritten = 0;

        smBiosDataSize = GetSystemFirmwareTable('RSMB', 0, NULL, 0);

        smBiosData = (RawSMBIOSData*)HeapAlloc(GetProcessHeap(), 0, smBiosDataSize);

        bytesWritten = GetSystemFirmwareTable('RSMB', 0, smBiosData, smBiosDataSize);

        size_t hash = simple_hash((char*)smBiosData, bytesWritten);

        nlohmann::json table1;

        table1["smbios_hash"] = hash;
        table1["smbios_length"] = bytesWritten;

        data.push_back(table1);

        //WMIC::DoRequest("SELECT * FROM Win32_BIOS", "SerialNumber", [&data](const char* value) {
        //    data["smbios"] = value;
        //});

        GetRegBinary(HKEY_LOCAL_MACHINE, XOR("SYSTEM\\CurrentControlSet\\Services\\mssmbios\\Data"), XOR("SMBiosData"), {

            size_t hash2 = simple_hash((char*)value, length);

            nlohmann::json table2;

            table2["smbios_hash"] = hash2;
            table2["smbios_length"] = length;

            data.push_back(table2);
        });
    }
)