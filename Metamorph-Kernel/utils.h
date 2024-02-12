#pragma once

#include "spoofer.h"

namespace Utils
{
	PVOID GetModuleBase(const char* moduleName);
	bool CheckMask(const char* base, const char* pattern, const char* mask);
	PVOID FindPattern(PVOID base, int length, const char* pattern, const char* mask);
	PVOID FindPatternImage(PVOID base, const char* pattern, const char* mask);
	void RandomText(char* text, const int length);
	void RandomBytes(char* text, const int length);

	PWCHAR TrimGUID(PWCHAR guid, DWORD max);
	PVOID SafeCopy(PVOID src, DWORD size);
	VOID ChangeIoc(PIO_STACK_LOCATION ioc, PIRP irp, PIO_COMPLETION_ROUTINE routine);
}
