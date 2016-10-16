#include "stdafx.h"

CSGO::Client* CSGO::GetClient()
{
	return (Client*)GetInterface("client.dll", "VClient017");
}

void* CSGO::GetClientHandle()
{
	static void *ret = GetModuleHandleA("client.dll");
	return (void*)ret;
}
