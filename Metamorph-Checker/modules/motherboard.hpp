
#include "module.h"

DEFINE_MODULE(Motherboard,
    {
        WMIC::DoRequest(XOR("SELECT * FROM Win32_BaseBoard WHERE SerialNumber IS NOT NULL"), XOR("SerialNumber"), [&data](const char* value) {
            data["serial"] = value;
        });
    }
)