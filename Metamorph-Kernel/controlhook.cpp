#include "controlhook.h"

namespace ControlHook {
	_SWAPS SWAPS = { 0 };
}

NTSTATUS ControlHook::AppendSwap(UNICODE_STRING name, PDRIVER_DISPATCH* swap, NTSTATUS(*hook)(PDEVICE_OBJECT device, PIRP irp), PDRIVER_DISPATCH* original) {

	PSWAP entry = &SWAPS.Buffer[SWAPS.Length++];

	entry->Swap = (PVOID*)swap;
	entry->Original = (PDRIVER_DISPATCH)InterlockedExchangePointer(entry->Swap, hook);
	entry->Name = name;

	*original = entry->Original;

	LOG(XOR("Swapped %wZ\n"), &name);

	return STATUS_SUCCESS;
}

// Swaps MJ device control and appends it to swap list on success
NTSTATUS ControlHook::SwapControl(UNICODE_STRING driver, NTSTATUS(*hook)(PDEVICE_OBJECT device, PIRP irp), PDRIVER_DISPATCH* original) {
	UNICODE_STRING str = driver;
	PDRIVER_OBJECT object = 0;
	NTSTATUS _status = ObReferenceObjectByName(&str, OBJ_CASE_INSENSITIVE, 0, 0, *IoDriverObjectType, KernelMode, 0, (PVOID*)&object);
	if (NT_SUCCESS(_status)) {
		AppendSwap(str, &object->MajorFunction[IRP_MJ_DEVICE_CONTROL], hook, original);
		ObDereferenceObject(object);
	} else {
		ERROR(XOR("Failed to get %wZ: %p !\n"), &str, _status);

		return STATUS_UNSUCCESSFUL;
	}

	return STATUS_SUCCESS;
}

NTSTATUS ControlHook::Shutdown() {

	for (DWORD i = 0; i < SWAPS.Length; ++i) {
		PSWAP s = (PSWAP)&SWAPS.Buffer[i];
		if (s->Swap && s->Original) {
			InterlockedExchangePointer(s->Swap, s->Original);
			LOG(XOR("UnSwapped: %wZ\n"), &s->Name);
		}
	}

	return STATUS_SUCCESS;
}