#include "stdafx.h"

using namespace std;

void Entrypoint(HMODULE hModule)
{
	g_pVars = new Netvars(); // Netvars
	luacs = new LuaCS();

	MessageBoxA(0, std::to_string( g_pVars->GetOffset("DT_BasePlayer", "m_Local") ).data(), "", 0);

	while (true)
	{
		Sleep(10);

		if (GetAsyncKeyState(VK_DELETE))
		{
			break;
		}
	}

	FreeLibraryAndExitThread(hModule, 0);
}
