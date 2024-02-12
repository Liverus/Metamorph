
#include "spoofer.h"

bool remove_file(const char* name) {

	std::filesystem::path filename = name;

	static auto userdata_path = std::filesystem::temp_directory_path()
		.parent_path()
		.parent_path()
		.parent_path();

	const char* roaming = XOR("Roaming");

	static auto roaming_path = userdata_path / roaming;

	auto tracker = roaming_path / filename;

	if (!std::filesystem::exists(tracker)) {
		return false;
	}

	if (std::filesystem::remove_all(tracker) == -1) {
		Error(XOR("  Failed to remove %s"), filename.u8string());

		return false;
	}

	Success(XOR("  Removed file %s"), filename.u8string());

	return true;
}

DEFINE_MODULE(SCPSL,
	{
		int count = 0;

		count += remove_file(XOR("jun-takahashi"));
		count += remove_file(XOR("blue-helmets"));
		count += remove_file(XOR("mongo-db"));
		count += remove_file(XOR("ts-modmail"));

		if (count == 0) {
			Success(XOR("  No tracker found or removed"));
		} else {
			Success(XOR("  Removed %d tracers"), count);
		}
	}
)