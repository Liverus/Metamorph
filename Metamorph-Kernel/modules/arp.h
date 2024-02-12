#pragma once

#include "../spoofer.h"

#define IOCTL_NSI_PROXY_ARP (0x0012001B)
#define NSI_PARAMS_ARP (11)

typedef struct _NSI_STRUCTURE_ENTRY {
	ULONG IpAddress;
	UCHAR Unknown[52];
} NSI_STRUCTURE_ENTRY, * PNSI_STRUCTURE_ENTRY;

typedef struct _NSI_STRUCTURE_2 {
	UCHAR Unknown[16];
	UCHAR IPAddress[4];
	UCHAR Unknown2[4];
	PNSI_STRUCTURE_ENTRY SubEntry;
	UCHAR Unknown3[8];
	PNSI_STRUCTURE_ENTRY SubEntry2;
} NSI_STRUCTURE_2, * PNSI_STRUCTURE_2;

typedef struct _NSI_STRUCTURE_1 {
	UCHAR _padding_0[24];
	ULONG Type;
	UCHAR _padding_1[8];
	PNSI_STRUCTURE_2 Entries;
	SIZE_T EntrySize;
	UCHAR Unknown2[48];
	SIZE_T NumberOfEntries;
} NSI_STRUCTURE_1, * PNSI_STRUCTURE_1;


// dt ndis!_NDIS_IF_BLOCK
typedef struct _NDIS_IF_BLOCK {
	char _padding_0[0x464];
	IF_PHYSICAL_ADDRESS_LH ifPhysAddress; // 0x464
	IF_PHYSICAL_ADDRESS_LH PermanentPhysAddress; // 0x486
} NDIS_IF_BLOCK, * PNDIS_IF_BLOCK;

typedef struct _KSTRING {
	char _padding_0[0x10];
	WCHAR Buffer[1]; // 0x10 at least
} KSTRING, * PKSTRING;

// dt ndis!_NDIS_FILTER_BLOCK
typedef struct _NDIS_FILTER_BLOCK {
	char _padding_0[0x8];
	struct _NDIS_FILTER_BLOCK* NextFilter; // 0x8
	char _padding_1[0x18];
	PKSTRING FilterInstanceName; // 0x28
} NDIS_FILTER_BLOCK, * PNDIS_FILTER_BLOCK;

namespace ARP {
	NTSTATUS Spoof();
}