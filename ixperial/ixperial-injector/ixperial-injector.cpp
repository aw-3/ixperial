// ixperial-injector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	DWORD pid = 0;
	
	do
	{
		pid = ProcessHelper::GetPIDFromName("csgo.exe");
		Sleep(100);
	} while (!pid);

	char DllPath[MAX_PATH] = { 0 };
	GetFullPathName("ixperial-main.dll", MAX_PATH, DllPath, NULL);

	ProcessHelper::InjectDLL(DllPath, pid, ProcessHelper::DLL_INJECTION_CRT);

    return 0;
}

