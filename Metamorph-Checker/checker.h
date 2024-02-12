#pragma once

#include <iostream>
#include <windows.h>
#include <winnls.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <filesystem>
#include <comdef.h>
#include <Wbemidl.h>
#include <sddl.h>
#include <ntstatus.h>
#include <windef.h>

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "IPHLPAPI.lib")

#include "../Shared/skcrypter/skcrypter.h"

#define XOR(...) skCrypt(__VA_ARGS__)

#include "../Shared/console/console.h"
#include "../Shared/json/json.hpp"
#include "../Shared/registry/registry.hpp"
#include "../Shared/hash/hash.h"

#include "wmic.h"
#include "module.h"

namespace Checker {
	BOOL Initialize();
}
