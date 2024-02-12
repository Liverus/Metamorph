#pragma once

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "../Shared/skcrypter/skcrypter.h"

#define XOR(...) skCrypt(__VA_ARGS__)
#define XORByte(...) skCryptByte(__VA_ARGS__)

#include "../Shared/kdmapper/kdmapper.hpp"

#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <ntstatus.h>
#include <filesystem>
#include <ctime>
#include <rpc.h>

#pragma comment(lib, "Rpcrt4.lib")

#include "../Shared/console/console.h"
#include "../Shared/vdf_parser/vdf_parser.hpp"
#include "../Shared/registry/registry.hpp"
#include "../Shared/hash/hash.h"

#include "util.h"
#include "module.h"

namespace Spoofer {
	NTSTATUS LoadUsermode();
	NTSTATUS LoadKernelmode();
}