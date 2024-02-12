#include "spoofer.h"

#include "modules/windows.hpp"
#include "modules/scpsl.hpp"
#include "modules/steam.hpp"

#include "driver.h"

NTSTATUS Spoofer::LoadUsermode() {

	DWORDLONG Seed = time(NULL);
	srand(Seed);

	LOAD_MODULE(Windows);
	LOAD_MODULE(SCPSL);
	LOAD_MODULE(Steam);

	Success(XOR("Stopping services.."));

	Util::StopProcess(XOR("WmiPrvSE.exe"));
	
	std::system(XOR("vssadmin delete shadows /All /Quiet > nul 2>&1"));
	std::system(XOR("net stop winmgmt /Y > nul 2>&1"));

	return STATUS_SUCCESS;
}

NTSTATUS Spoofer::LoadKernelmode() {

	HANDLE iqvw64e_device_handle = intel_driver::Load();

	if (iqvw64e_device_handle == INVALID_HANDLE_VALUE) {
		Error(XOR("Failed to load vulnerable driver"));

		return STATUS_UNSUCCESSFUL;
	}

	Success(XOR("Loaded Vulnerable Driver"));

	NTSTATUS exitCode = 0;
	
	if (!kdmapper::MapDriver(iqvw64e_device_handle, (BYTE*)driver, 0, 0, false, true, kdmapper::AllocationMode::AllocateMdl, false, NULL, &exitCode)) {
		Error(XOR("Failed to map driver"));
	
		intel_driver::Unload(iqvw64e_device_handle);

		return STATUS_UNSUCCESSFUL;
	}

	Success(XOR("Mapped Spoofer"));

	if (!intel_driver::Unload(iqvw64e_device_handle)) {
		Error(XOR("Failed to fully unload vulnerable driver"));

		return STATUS_UNSUCCESSFUL;
	}

	Success(XOR("Unloaded Vulnerable Driver"));

	return STATUS_SUCCESS;
}