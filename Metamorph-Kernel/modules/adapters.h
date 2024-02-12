#pragma once

#include "../spoofer.h"

typedef struct _NIC_DRIVER {
	PDRIVER_OBJECT DriverObject;
	PDRIVER_DISPATCH Original;
} NIC_DRIVER, * PNIC_DRIVER;

struct _NICS {
	DWORD Length;
	NIC_DRIVER Drivers[0xFF];
};

typedef struct _ADAPTER_ENTRY {
	UCHAR OriginalMacAddress[6];
	UCHAR NewMacAddress[6];
} ADAPTER_ENTRY, * PADAPTER_ENTRY;

struct _ADAPTERS {
	DWORD Length;
	ADAPTER_ENTRY Addresses[0xFF];
};

namespace Adapters {

	NTSTATUS Spoof();
	NTSTATUS SpoofAddress(UCHAR* addr);

	extern _NICS NICs;
	extern _ADAPTERS Cache;
}