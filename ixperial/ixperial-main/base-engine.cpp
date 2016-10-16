#include "stdafx.h"

CSGO::IVEngine* CSGO::GetEngine()
{
	return (IVEngine*)GetInterface("engine.dll", "VEngineClient014");
}
