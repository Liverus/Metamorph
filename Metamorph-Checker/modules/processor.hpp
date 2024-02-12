
#include "module.h"

DEFINE_MODULE(Processor,
    {
        WMIC::DoRequest(XOR("SELECT * FROM Win32_Processor WHERE Name IS NOT NULL"), XOR("Name"), [&data](const char* value) {
            data = value;
        });
    }
)