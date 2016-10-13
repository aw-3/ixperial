#include "stdafx.h"
#include <TlHelp32.h>

int ProcessHelper::GetPIDFromName(std::string processName)
{
	int ret = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 pe = { 0 };
	pe.dwSize = sizeof(pe);

	if (Process32First(snapshot, &pe))
	{
		do
		{
			if (pe.szExeFile == processName)
			{
				ret = pe.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &pe));
	}

	CloseHandle(snapshot);

	return ret;
}

bool ProcessHelper::InjectDLL(std::string dllPath, int PID, DLL_INJECTION_TYPE method)
{
	if (method != DLL_INJECTION_CRT)
	{
		return false; // only supported method atm
	}

	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

	if (process == INVALID_HANDLE_VALUE)
	{
		std::cout << GetLastError() << std::endl;
		return false;
	}

	// TODO: better error handling would be nice

	LPVOID LoadLibAddress = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	LPVOID RemoteString = (LPVOID)VirtualAllocEx(process, NULL, dllPath.length(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(process, (LPVOID)RemoteString, dllPath.data(), dllPath.length(), NULL);
	CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddress, (LPVOID)RemoteString, NULL, NULL);
	CloseHandle(process);

	return true;
}
