
#include "spoofer.h"

int main(void) {

	SetTitle(XOR("[Metamorph]"));

	Success(XOR("Kernelmode"));

	if (!NT_SUCCESS(Spoofer::LoadKernelmode())) {
		Error(XOR("Failed to load Kernelmode"));

		Newline;

		SystemPause;

		return STATUS_UNSUCCESSFUL;
	}

	Success(XOR("Done"));

	Newline;

	Success(XOR("Usermode"));

	if (!NT_SUCCESS(Spoofer::LoadUsermode())) {
		Error(XOR("Failed to load Usermode"));

		Newline;

		SystemPause;

		return STATUS_UNSUCCESSFUL;
	}

	Success(XOR("Done"));
	
	Newline;

	Success(XOR("Metamorph has successfully loaded"));

	Newline;

	Warning(XOR("Make sure all your network adapters are already loaded"));
	Warning(XOR("You must create a new user for a full unban"));

	Newline;

	SystemPause;

	return STATUS_SUCCESS;
}