#include "stdafx.h"

bool LuaEntity::IsDormant() const
{
	static int offset = g_pVars->GetOffset("BaseEntity", "m_bDormant");
	return *(bool*)((int)this + offset);
}
