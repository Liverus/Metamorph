#pragma once

#include "checker.h"

namespace WMIC
{
	void Connect();
	void DoRequest(const char* req, const char* key, const std::function<void(const char* value)>&callback);
	void Disconnect();

	extern IWbemLocator* pLoc;
	extern IWbemServices* pSvc;
};

