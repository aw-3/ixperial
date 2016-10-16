#pragma once

namespace VMTHook {
	void Hook(std::string name, void* tablePtr);

	void* WriteVMTMethod(std::string name, unsigned int idx, void* newMethod);

	void EnableHook(std::string name);
	void DisableHook(std::string name);

	void RestoreVMTMethod(std::string name, unsigned int idx);
	void RestoreVMT(std::string name);
}