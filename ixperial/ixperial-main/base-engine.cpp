#include "stdafx.h"

CSGO::IVEngine* CSGO::GetEngine()
{
	static IVEngine *ret = (IVEngine*)GetInterface("engine.dll", "VEngineClient014");
	return ret;
}

void * CSGO::GetEngineHandle()
{
	static void *ret = GetModuleHandleA("engine.dll");
	return (void*)ret;
}

CSGO::IVClientState* CSGO::GetClientState()
{
	static int clientStatePtr = sigScan((int)GetEngineHandle(), "\xA1\x00\x00\x00\x00\xF3\x0F\x11\x80\x00\x00\x00\x00\xD9\x46\x04\xD9\x05\x00\x00\x00\x00");
	IVClientState **ret = *(IVClientState***)(clientStatePtr + 1);

	if (!ret || !*ret) return nullptr;

	return *ret;
}

void CSGO::IVEngine::ExecuteClientCmd(const char * szCmdString)
{
	static auto fn = GetVirtualMethod<void(__thiscall*)(void*, const char*)>(this, 108);
	return fn(this, szCmdString);
}

float* CSGO::IVClientState::GetViewAngles()
{
	return (float*)((char*)this + 0x4D0C); // TODO: stop using offsets for this
}
