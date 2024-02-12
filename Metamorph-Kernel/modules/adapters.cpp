
#include "adapters.h"

namespace Adapters {
	_NICS NICs = { 0 };
	_ADAPTERS Cache = { 0 };
}

NTSTATUS NICIoc(PDEVICE_OBJECT device, PIRP irp, PVOID context) {
	if (context) {
		IOC_REQUEST request = *(PIOC_REQUEST)context;
		ExFreePool(context);

		if (irp->MdlAddress) {
			// Utils::RandomText((char*)MmGetSystemAddressForMdl(irp->MdlAddress), 6);

			Adapters::SpoofAddress((UCHAR*)MmGetSystemAddressForMdl(irp->MdlAddress));

			LOG(XOR("handled NICIoc\n"));
		}

		if (request.OldRoutine && irp->StackCount > 1) {
			return request.OldRoutine(device, irp, request.OldContext);
		}
	}

	return STATUS_SUCCESS;
}

NTSTATUS NICControl(PDEVICE_OBJECT device, PIRP irp) {
	for (DWORD i = 0; i < Adapters::NICs.Length; ++i) {
		PNIC_DRIVER driver = &Adapters::NICs.Drivers[i];

		if (driver->Original && driver->DriverObject == device->DriverObject) {
			PIO_STACK_LOCATION ioc = IoGetCurrentIrpStackLocation(irp);
			switch (ioc->Parameters.DeviceIoControl.IoControlCode) {
			case IOCTL_NDIS_QUERY_GLOBAL_STATS: {
				switch (*(PDWORD)irp->AssociatedIrp.SystemBuffer) {
				case OID_802_3_PERMANENT_ADDRESS:
				case OID_802_3_CURRENT_ADDRESS:
				case OID_802_5_PERMANENT_ADDRESS:
				case OID_802_5_CURRENT_ADDRESS:
					Utils::ChangeIoc(ioc, irp, NICIoc);
					break;
				}
				LOG(XOR("Unhandled Request: %i"), *(PDWORD)irp->AssociatedIrp.SystemBuffer);
				break;
			}
			}

			return driver->Original(device, irp);
		}
	}

	return STATUS_SUCCESS;
}

NTSTATUS Adapters::SpoofAddress(UCHAR* addr) {

	for (int i = 0; i < Cache.Length; i++) {
		auto entry = &Cache.Addresses[i];

		if (memcmp(entry->OriginalMacAddress, addr, 6) == 0) {

			memcpy(addr, entry->NewMacAddress, 6);

			LOG(XOR("Found MAC Address: %02x-%02x-%02x-%02x-%02x-%02x to %02x-%02x-%02x-%02x-%02x-%02x\n"),
				entry->OriginalMacAddress[0], entry->OriginalMacAddress[1], entry->OriginalMacAddress[2],
				entry->OriginalMacAddress[3], entry->OriginalMacAddress[4], entry->OriginalMacAddress[5],
				entry->NewMacAddress[0], entry->NewMacAddress[1], entry->NewMacAddress[2],
				entry->NewMacAddress[3], entry->NewMacAddress[4], entry->NewMacAddress[5]
			);
			
			return STATUS_SUCCESS;
		}
	}

	auto entry = &Cache.Addresses[Cache.Length++];

	memcpy(entry->OriginalMacAddress, addr, 6);

	Utils::RandomBytes((char*)addr, 6);

	memcpy(entry->NewMacAddress, addr, 6);

	LOG(XOR("Added MAC Address: %02x-%02x-%02x-%02x-%02x-%02x to %02x-%02x-%02x-%02x-%02x-%02x\n"), 
		entry->OriginalMacAddress[0], entry->OriginalMacAddress[1], entry->OriginalMacAddress[2],
		entry->OriginalMacAddress[3], entry->OriginalMacAddress[4], entry->OriginalMacAddress[5],
		entry->NewMacAddress[0], entry->NewMacAddress[1], entry->NewMacAddress[2],
		entry->NewMacAddress[3], entry->NewMacAddress[4], entry->NewMacAddress[5]
	);

	return STATUS_SUCCESS;
}

NTSTATUS Adapters::Spoof() {

	PVOID base = Utils::GetModuleBase(XOR("ndis.sys"));
	if (!base) {
		ERROR(XOR("! failed to get ndis.sys !\n"));
		return STATUS_SUCCESS;
	}

	PNDIS_FILTER_BLOCK ndisGlobalFilterList = (PNDIS_FILTER_BLOCK)Utils::FindPatternImage(base, XOR("\x40\x8A\xF0\x48\x8B\x05"), XOR("xxxxxx"));
	if (ndisGlobalFilterList) {
		PDWORD ndisFilter_IfBlock = (PDWORD)Utils::FindPatternImage(base, XOR("\x48\x85\x00\x0F\x84\x00\x00\x00\x00\x00\x8B\x00\x00\x00\x00\x00\x33"), XOR("xx?xx?????x???xxx"));
		if (ndisFilter_IfBlock) {
			DWORD ndisFilter_IfBlock_offset = *(PDWORD)((PBYTE)ndisFilter_IfBlock + 12);

			ndisGlobalFilterList = (PNDIS_FILTER_BLOCK)((PBYTE)ndisGlobalFilterList + 3);
			ndisGlobalFilterList = *(PNDIS_FILTER_BLOCK*)((PBYTE)ndisGlobalFilterList + 7 + *(PINT)((PBYTE)ndisGlobalFilterList + 3));

			DWORD count = 0;
			for (PNDIS_FILTER_BLOCK filter = ndisGlobalFilterList; filter; filter = filter->NextFilter) {
				PNDIS_IF_BLOCK block = *(PNDIS_IF_BLOCK*)((PBYTE)filter + ndisFilter_IfBlock_offset);
				if (block) {
					PWCHAR copy = (PWCHAR)Utils::SafeCopy(filter->FilterInstanceName->Buffer, MAX_PATH);
					if (copy) {
						WCHAR adapter[MAX_PATH] = { 0 };
						swprintf(adapter, XORW(L"\\Device\\%ws"), Utils::TrimGUID(copy, MAX_PATH / 2));
						ExFreePool(copy);

						LOG(XOR("found NIC %ws\n"), adapter);

						UNICODE_STRING name = { 0 };
						RtlInitUnicodeString(&name, adapter);

						PFILE_OBJECT file = 0;
						PDEVICE_OBJECT device = 0;

						NTSTATUS status = IoGetDeviceObjectPointer(&name, FILE_READ_DATA, &file, &device);
						if (NT_SUCCESS(status)) {
							PDRIVER_OBJECT driver = device->DriverObject;
							if (driver) {
								BOOL exists = FALSE;
								for (DWORD i = 0; i < NICs.Length; ++i) {
									if (NICs.Drivers[i].DriverObject == driver) {
										exists = TRUE;
										break;
									}
								}

								if (exists) {
									LOG(XOR("%wZ already swapped\n"), &driver->DriverName);
								}
								else {
									PNIC_DRIVER nic = &NICs.Drivers[NICs.Length];
									nic->DriverObject = driver;

									ControlHook::AppendSwap(driver->DriverName, &driver->MajorFunction[IRP_MJ_DEVICE_CONTROL], NICControl, &nic->Original);

									++NICs.Length;
								}
							}

							// Indirectly dereferences device object
							ObDereferenceObject(file);
						}
						else {
							ERROR(XOR("! failed to get %wZ: %p !\n"), &name, status);
						}
					}

					// Current MAC
					PIF_PHYSICAL_ADDRESS_LH addr = &block->ifPhysAddress;
					Utils::RandomBytes((char*)addr->Address, addr->Length);
					addr = &block->PermanentPhysAddress;
					Utils::RandomBytes((char*)addr->Address, addr->Length);

					++count;
				}
			}

			LOG(XOR("handled %d MACs\n"), count);
		}
		else {
			ERROR(XOR("! failed to find ndisFilter_IfBlock !\n"));
		}
	}
	else {
		ERROR(XOR("! failed to find ndisGlobalFilterList !\n"));
	}

	return STATUS_SUCCESS;
}