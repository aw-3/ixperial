#include "stdafx.h"

using namespace std;

void Entrypoint(HMODULE hModule)
{
	g_pVars = new Netvars();
	luacs = new LuaCS();

	CSGO::LoadEngineHooks();

	luacs->ExecuteAllScripts();

	while (true)
	{
		Sleep(10);

		if (GetAsyncKeyState(VK_DELETE) & 1) // reloading scripts for debug purposes
		{
			delete luacs;
			luacs = new LuaCS();
			luacs->ExecuteAllScripts();
		}
	}

	CSGO::UnloadEngineHooks();

	FreeLibraryAndExitThread(hModule, 0); // useless as shit because lua is still loaded into the process
}
