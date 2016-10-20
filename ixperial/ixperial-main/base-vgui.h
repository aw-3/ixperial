#pragma once

namespace CSGO {
	
	void* GetVGUI()
	{
		return (void*)GetInterface("vgui2.dll", "VGUI_Panel009");
	}
}