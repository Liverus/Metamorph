#pragma once

#include "spoofer.h"

extern "C" POBJECT_TYPE* IoDriverObjectType;

// Appends swap to swap list
#define TAppendSwap(name, swap, hook, original) { \
	UNICODE_STRING _n = name; \
	ControlHook::PSWAP _s = &ControlHook::SWAPS.Buffer[ControlHook::SWAPS.Length++]; \
	*(PVOID *)&original = _s->Original = InterlockedExchangePointer((PVOID *)(_s->Swap = (PVOID *)swap), (PVOID)hook); \
	_s->Name = _n; \
	LOG(XOR("swapped %wZ\n"), &_n); \
}

// Swaps MJ device control and appends it to swap list on success
#define TSwapControl(driver, hook, original) { \
	UNICODE_STRING str = driver; \
	PDRIVER_OBJECT object = 0; \
	NTSTATUS _status = ObReferenceObjectByName(&str, OBJ_CASE_INSENSITIVE, 0, 0, *IoDriverObjectType, KernelMode, 0, (PVOID*)&object); \
	if (NT_SUCCESS(_status)) { \
		TAppendSwap(str, &object->MajorFunction[IRP_MJ_DEVICE_CONTROL], hook, original); \
		ObDereferenceObject(object); \
	} else { \
		LOG(XOR("! failed to get %wZ: %p !\n"), &str, _status); \
	} \
}

namespace ControlHook
{
	typedef struct _SWAP {
		UNICODE_STRING Name;
		PVOID* Swap;
		PDRIVER_DISPATCH Original;
	} SWAP, * PSWAP;

	static struct _SWAPS {
		SWAP Buffer[0xFF];
		ULONG Length;
	};

	NTSTATUS SwapControl(UNICODE_STRING driver, NTSTATUS(*hook)(PDEVICE_OBJECT device, PIRP irp), PDRIVER_DISPATCH* original);
	NTSTATUS AppendSwap(UNICODE_STRING name, PDRIVER_DISPATCH* swap, NTSTATUS(*hook)(PDEVICE_OBJECT device, PIRP irp), PDRIVER_DISPATCH* original);
	NTSTATUS Shutdown();

	extern _SWAPS SWAPS;
};