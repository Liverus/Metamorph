#pragma once

#include "spoofer.h"

#define DEFINE_MODULE(name, body) class MODULE_ ## name : public MetaModule { \
public: \
	NTSTATUS Execute() { \
	body \
	return STATUS_SUCCESS; \
	} \
};

#define LOAD_MODULE(name) Success(XOR("Module: "#name)); (new MODULE_ ## name())->Execute();

class MetaModule
{
	void Execute() {};
};
