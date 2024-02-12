/*
 * Mutante
 * Made by Samuel Tulach
 * https://github.com/SamuelTulach/mutante
 */

#include "spoofer.h"

// #define SC_LOAD

#ifdef SC_LOAD

VOID DriverUnload(PDRIVER_OBJECT driver) {
	UNREFERENCED_PARAMETER(driver);

	Spoofer::Shutdown();

	LOG(XOR("Driver unloaded.\n"));
}

#endif

/**
 * \brief Driver's main entry point
 * \param object Pointer to driver object (invalid when manual mapping)
 * \param registry Registry path (invalid when manual mapping)
 * \return Status of the driver execution
 */
extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING registry)
{
	UNREFERENCED_PARAMETER(registry);

#ifdef SC_LOAD
	driver->DriverUnload = DriverUnload;
#else
	UNREFERENCED_PARAMETER(driver);
#endif

	LOG(XOR("Driver loaded.\n"));

	Spoofer::Initialize();

	LOG(XOR("Done.\n"));

	return STATUS_SUCCESS;
}