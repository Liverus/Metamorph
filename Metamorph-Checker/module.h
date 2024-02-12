#pragma once

#include "checker.h"

#define DEFINE_MODULE(name, body) class MODULE_ ## name : public MetaModule { \
public: \
	NTSTATUS Execute(nlohmann::json& data) { \
	body \
	return STATUS_SUCCESS; \
	} \
};

#define LOAD_MODULE(name) nlohmann::json json_ ## name;\
(new MODULE_ ## name())->Execute(json_ ## name); \
data[#name] = json_ ## name;

class MetaModule
{
	NTSTATUS Execute(nlohmann::json& data) {};
};
