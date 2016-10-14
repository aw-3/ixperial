#include "stdafx.h"

using namespace std;

void Entrypoint(HMODULE hModule)
{
	g_pVars = new Netvars(); // Netvars

	MessageBoxA(0, std::to_string( g_pVars->GetOffset("DT_BasePlayer", "m_Local") ).data(), "", 0);
	FreeLibraryAndExitThread(hModule, 0);
}
