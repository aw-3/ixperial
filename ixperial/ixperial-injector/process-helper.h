#pragma once

namespace ProcessHelper {
	enum DLL_INJECTION_TYPE
	{
		DLL_INJECTION_CRT = 0,
		DLL_INJECTION_MMAP,
		DLL_INJECTION_LDR
	};

	int GetPIDFromName(std::string processName);
	bool InjectDLL(std::string dllPath, int PID, DLL_INJECTION_TYPE method);
}