#include "stdafx.h"
#include "lua-ucmd.h"

int LuaUCmd::GetButtons() const
{
	return pCmd ? pCmd->buttons : 0;
}

void LuaUCmd::SetButtons(const int &buttons)
{
	if (pCmd)
		pCmd->buttons = buttons;
}
