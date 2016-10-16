#include "stdafx.h"

CSGO::Client* CSGO::GetClient()
{
	return (Client*)GetInterface("client.dll", "VClient017");
}