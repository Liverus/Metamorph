
#include "module.h"

DEFINE_MODULE(Driver,
    {
        WMIC::DoRequest(XOR("SELECT * FROM Win32_SystemDriver WHERE Name IS NOT NULL"), XOR("Name"), [&data](const char* value) {
            data.push_back(value);
        });
    }
)