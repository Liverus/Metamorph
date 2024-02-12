#pragma once

#include "../spoofer.h"

namespace Disks
{
	NTSTATUS Spoof();

	PDEVICE_OBJECT GetRaidDevice(const wchar_t* deviceName);
	NTSTATUS DiskLoop(PDEVICE_OBJECT deviceArray, RaidUnitRegisterInterfaces registerInterfaces);
	NTSTATUS DisableSmart();
	void DisableSmartBit(PRAID_UNIT_EXTENSION extension);
}