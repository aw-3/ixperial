#include "stdafx.h"

void * CSGO::GetVGUI()
{
	return (void*)GetInterface("vgui2.dll", "VGUI_Panel009");
}
