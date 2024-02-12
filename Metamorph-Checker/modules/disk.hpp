
#include "module.h"

DEFINE_MODULE(Disk,
    {
        nlohmann::json table1;

        WMIC::DoRequest(XOR("SELECT * FROM Win32_PhysicalMedia WHERE SerialNumber IS NOT NULL"), XOR("SerialNumber"), [&table1](const char* value) {
            table1.push_back(value);
        });

        nlohmann::json table2;
            
        WMIC::DoRequest(XOR("SELECT * FROM Win32_DiskDrive WHERE SerialNumber IS NOT NULL"), XOR("SerialNumber"), [&table2](const char* value) {
            table2.push_back(value);
        });

        data.push_back(table1);
        data.push_back(table2);
    }
)