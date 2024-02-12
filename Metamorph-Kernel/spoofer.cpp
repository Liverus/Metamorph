#include "spoofer.h"

NTSTATUS Spoofer::Initialize() {

	// Disk
	Disks::DisableSmart();
	Disks::Spoof();

	// SMBios
	SMBios::Spoof();

	// Network
	ARP::Spoof();
	Adapters::Spoof();

	return STATUS_SUCCESS;
}

NTSTATUS Spoofer::Shutdown() {

	ControlHook::Shutdown();

	return STATUS_SUCCESS;
}