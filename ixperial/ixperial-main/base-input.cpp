#include "stdafx.h"
#include "base-input.h"

CSGO::IVInput* CSGO::GetInput()
{
	static IVInput *ret = *(IVInput**)(sigScan((int)GetClientHandle(), "\xB9\x00\x00\x00\x00\xFF\x75\x08\xE8\x00\x00\x00\x00\x8B\x06") + 1);
	return ret;
}
