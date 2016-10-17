#include "stdafx.h"

using namespace std;

void Entrypoint(HMODULE hModule)
{
	g_pVars = new Netvars(); // Netvars
	luacs = new LuaCS();

	CSGO::LoadEngineHooks();
	//MessageBoxA(0, std::to_string( g_pVars->GetOffset("DT_BasePlayer", "m_Local") ).data(), "", 0);

	//luaL_dostring(luacs->L, "bindEvent('createmove', function() alert(tostring(csgo.entity(1).health)); end)");
	//luaL_dostring(luacs->L, "bindEvent('createmove', function() csgo.clientcmd('echo hi') end)");

	// execute everything
	luacs->ExecuteAllScripts();

	while (true)
	{
		Sleep(10);

		if (GetAsyncKeyState(VK_DELETE))
		{
			break;
		}
	}

	CSGO::UnloadEngineHooks();

	FreeLibraryAndExitThread(hModule, 0); // useless as shit because lua is still loaded into the process
}
