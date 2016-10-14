#pragma once

#include "dt_recv.h"

namespace CSGO {
	typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

	void* GetInterface(std::string dll, std::string name);
}