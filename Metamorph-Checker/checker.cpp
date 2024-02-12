#include "checker.h"

#include "modules/arp.hpp"
#include "modules/network.hpp"
#include "modules/motherboard.hpp"
#include "modules/disk.hpp"
#include "modules/steam.hpp"
#include "modules/driver.hpp"
#include "modules/processor.hpp"
#include "modules/sid.hpp"
#include "modules/windows.hpp"
#include "modules/smbios.hpp"

BOOL Checker::Initialize() {

	nlohmann::json data;

	WMIC::Connect();

	LOAD_MODULE(Arp);
	LOAD_MODULE(Network);
	LOAD_MODULE(Disk);
	LOAD_MODULE(Motherboard);
	LOAD_MODULE(Processor);
	LOAD_MODULE(UserSID);
	LOAD_MODULE(Steam);
	LOAD_MODULE(Windows);
	LOAD_MODULE(Smbios);
	// LOAD_MODULE(Driver); Not very useful, disabling for now

	WMIC::Disconnect();

	std::cout << data.dump(4) << std::endl;

	return TRUE;
}