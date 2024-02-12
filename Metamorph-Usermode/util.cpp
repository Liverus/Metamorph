
#include "util.h"

NTSTATUS Util::StopProcess(const char* processName) {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot) {
		PROCESSENTRY32 entry = { 0 };
		entry.dwSize = sizeof(entry);
		if (Process32First(snapshot, &entry)) {
			do {
				if (strcmp(entry.szExeFile, processName) == 0) {
					HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, 0, entry.th32ProcessID);
					if (INVALID_HANDLE_VALUE != process) {
						TerminateProcess(process, 0);
						CloseHandle(process);
					}

					break;
				}
			} while (Process32Next(snapshot, &entry));
		}

		CloseHandle(snapshot);
	}

	return STATUS_SUCCESS;
}