#include "stdafx.h"

CSGO::IVEngine* CSGO::GetEngine()
{
	static IVEngine *ret = (IVEngine*)GetInterface("engine.dll", "VEngineClient014");
	return ret;
}

void CSGO::IVEngine::ExecuteClientCmd(const char * szCmdString)
{
	static auto fn = GetVirtualMethod<void(__thiscall*)(void*, const char*)>(this, 108);
	return fn(this, szCmdString);
}
