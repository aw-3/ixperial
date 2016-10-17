#pragma once

#include "dt_recv.h"

namespace CSGO {
	typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

	void* GetInterface(std::string dll, std::string name);

	template<typename Function>
	Function GetVirtualMethod(void *base, int idx)
	{
		int** VTablePointer = (int**)base;
		int* VTableFunctionBase = *VTablePointer;
		int dwAddress = VTableFunctionBase[idx];
		return (Function)(dwAddress);
	}

	void LoadEngineHooks();
	void UnloadEngineHooks();
}