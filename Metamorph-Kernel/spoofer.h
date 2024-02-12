#pragma once

#include <ntifs.h>
#include <ntstrsafe.h>
#include <windef.h>
#include <ntimage.h>
#include "shared.h"
#include <stdarg.h>
#include "ipmib.h"
#include <ntddndis.h>

#define DISABLE_OUTPUT

#include "../Shared/skcrypter/skcrypter.h"

#define XOR(x) skCrypt(x)
#define XORW(x) skCryptW(x)

#include "controlhook.h"
#include "log.h"
#include "utils.h"

#include "modules/disks.h"
#include "modules/smbios.h"
#include "modules/arp.h"
#include "modules/adapters.h"

namespace Spoofer {
	NTSTATUS Initialize();
	NTSTATUS Shutdown();
}