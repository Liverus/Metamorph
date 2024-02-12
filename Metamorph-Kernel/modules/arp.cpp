
#include "arp.h"

PDRIVER_DISPATCH NsiControlOriginal = 0;

NTSTATUS NsiControl(PDEVICE_OBJECT device, PIRP irp) {

	PIO_STACK_LOCATION ioc = IoGetCurrentIrpStackLocation(irp);
	switch (ioc->Parameters.DeviceIoControl.IoControlCode) {
	case IOCTL_NSI_PROXY_ARP: {
		DWORD length = ioc->Parameters.DeviceIoControl.OutputBufferLength;

		NTSTATUS status = NsiControlOriginal(device, irp);

		PNSI_STRUCTURE_1 params = (PNSI_STRUCTURE_1)irp->UserBuffer;

		if (params && NSI_PARAMS_ARP == params->Type) {

			params->NumberOfEntries = 0;

			//LOG(XOR("Type: %d\n"), params->Type);
			//LOG(XOR("Length: %d\n"), length);

			//LOG(XOR("After\n"));

			//PrintBufferAsHex(irp->UserBuffer, 0xFF);

			// memset(irp->UserBuffer, 0xFF, length);

			// memset( (PVOID)((DWORD64)irp->UserBuffer + 112), 0x00, 255-112);

			// params->NumberOfEntries = 0;

			// LOG(XOR("Count: %d\n"), params->NumberOfEntries);


			//LOG(XOR("Handled ARP table (Length: %d)\n"), length);
		} else {
			// memset(irp->UserBuffer, 0x00, length);
		}

		return status;
	}
	}

	return NsiControlOriginal(device, irp);
}

NTSTATUS ARP::Spoof() {
	ControlHook::SwapControl(RTL_CONSTANT_STRING(L"\\Driver\\nsiproxy"), NsiControl, &NsiControlOriginal);

	return STATUS_SUCCESS;
}