#include "stdafx.h"

void* LuaEntity::GetEntityFromIndex(int i)
{
	static int *listPtr = (int*)sigScan((int)CSGO::GetClientHandle(), "\xBB\x00\x00\x00\x00\x83\xFF\x01\x0F\x8C\x00\x00\x00\x00\x3B\xF8");

	int *list = *(int**)(listPtr + 1);

	return *(int**)(list + (i * 0x10));
}

void* LuaEntity::GetLocalPlayer()
{
	return nullptr;
}

void LuaEntity::UpdateEntity() const
{
	if (!pEntity)
	{
		pEntity = (int)GetEntityFromIndex(idx);
	}
}

bool LuaEntity::IsDormant() const
{
	UpdateEntity(); if (!pEntity) return true;
	static int offset = g_pVars->GetOffset("BaseEntity", "m_bDormant");
	return *(bool*)(pEntity + offset);
}

int LuaEntity::GetTeam() const
{
	UpdateEntity(); if (!pEntity) return 0;
	static int offset = g_pVars->GetOffset("DT_BaseEntity", "m_iTeamNum");
	return *(int*)(pEntity + offset);
}

int LuaEntity::GetFlags() const
{
	UpdateEntity(); if (!pEntity) return 0;
	static int offset = g_pVars->GetOffset("DT_BasePlayer", "m_fFlags");
	return *(int*)(pEntity + offset);
}

int LuaEntity::GetHealth() const
{
	UpdateEntity(); if (!pEntity) return 0;
	static int offset = g_pVars->GetOffset("DT_BasePlayer", "m_iHealth");
	return *(int*)(pEntity + offset);
}
