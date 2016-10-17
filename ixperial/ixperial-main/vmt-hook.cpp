#include "stdafx.h"

struct HookEntry {
	std::string hookName;
	void* pVmt;
	void* origVmt;
	void* newVmt;

	unsigned int idxCount;
	bool isHooked;
};

std::vector<HookEntry*> g_Hooks;

void VMTHook::Hook(std::string name, void *tablePtr)
{
	for (auto it = g_Hooks.begin(); it != g_Hooks.end(); it++)
	{
		auto entry = *it;

		if (entry->hookName == name /*|| entry->pVmt == tablePtr*/)
			return; // already exists
	}

	HookEntry *vm = new HookEntry;

	vm->hookName = name;
	vm->isHooked = false;
	vm->pVmt = tablePtr;
	vm->origVmt = *(void**)tablePtr;

	// get method count
	try
	{
		for (vm->idxCount = 0; ((int*)vm->origVmt)[vm->idxCount]; vm->idxCount++) {
			if (((int*)vm->origVmt)[vm->idxCount] < 0x50000 || IsBadReadPtr((FARPROC)((int*)vm->origVmt)[vm->idxCount], 4) ||
				IsBadCodePtr((FARPROC)((int*)vm->origVmt)[vm->idxCount]))
				break;
		}
	}
	catch (...) // please god don't VAC ban me for exception handling
	{
		vm->idxCount++; // just to be sure we don't miss
	}
	

	// allocate new table
	vm->newVmt = new int[vm->idxCount];
	memcpy(vm->newVmt, vm->origVmt, sizeof(int) * vm->idxCount);

	g_Hooks.push_back(vm);
}

void* VMTHook::WriteVMTMethod(std::string name, unsigned int idx, void *newMethod)
{
	for (auto it = g_Hooks.begin(); it != g_Hooks.end(); it++)
	{
		auto entry = *it;

		if (entry->hookName == name)
		{
			((int*)entry->newVmt)[idx] = (int)newMethod;
			return (void*)((int*)entry->origVmt)[idx];
		}
	}
	return nullptr;
}

void VMTHook::EnableHook(std::string name)
{
	for (auto it = g_Hooks.begin(); it != g_Hooks.end(); it++)
	{
		auto entry = *it;

		if (entry->hookName == name && !entry->isHooked)
		{
			entry->isHooked = true;
			*(int*)entry->pVmt = (int)entry->newVmt;
		}
	}
}

void VMTHook::DisableHook(std::string name)
{
	for (auto it = g_Hooks.begin(); it != g_Hooks.end(); it++)
	{
		auto entry = *it;

		if (entry->hookName == name && entry->isHooked)
		{
			entry->isHooked = false;
			*(int*)entry->pVmt = (int)entry->origVmt;
		}
	}
}

void VMTHook::RestoreVMTMethod(std::string name, unsigned int idx)
{
	for (auto it = g_Hooks.begin(); it != g_Hooks.end(); it++)
	{
		auto entry = *it;

		if (entry->hookName == name)
		{
			((int*)entry->newVmt)[idx] = ((int*)entry->origVmt)[idx];
		}
	}
}

void VMTHook::RestoreVMT(std::string name)
{
	DisableHook(name);
}

