
#include "module.h"

DEFINE_MODULE(Network,
    {
        WMIC::DoRequest(XOR("SELECT * FROM Win32_NetworkAdapterConfiguration WHERE MACAddress IS NOT NULL"), XOR("MACAddress"), [&data](const char* value) {
            data.push_back(value);
        });
    }
)