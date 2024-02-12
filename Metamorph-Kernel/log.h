#pragma once

#include "spoofer.h"

#ifndef DISABLE_OUTPUT
	#define LOG(...) Log::Print("[Log] " ## __VA_ARGS__);
	#define ERROR(...) Log::Print("[Error] " ## __VA_ARGS__);
#else
	#define LOG(...);
	#define ERROR(...);
#endif

// #define RAWLOG(...) Log::Print(__VA_ARGS__);

namespace Log
{
	void Print(const char* text, ...);
}