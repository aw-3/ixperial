#include "stdafx.h"

void* LuaEntity::GetEntityFromIndex(int i)
{
	static void *listPtr = (int*)sigScan((int)CSGO::GetClientHandle(), "\xBB\x00\x00\x00\x00\x83\xFF\x01\x0F\x8C\x00\x00\x00\x00\x3B\xF8");

	int list = *(int*)((char*)listPtr + 1);

	return *(int**)(list + (i * 0x10));
}

RefCountedPtr<LuaEntity> LuaEntity::GetLocalPlayer()
{
	int localIndex = CSGO::GetEngine()->GetLocalPlayer();
	return RefCountedPtr<LuaEntity>(new LuaEntity(localIndex));
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

int LuaEntity::GetArmor() const
{
	return 0;
}

int LuaEntity::ShotsFired() const
{
	return 0;
}

void LuaEntity::SetFlags(const int& fl)
{
	UpdateEntity(); if (!pEntity) return;
	static int offset = g_pVars->GetOffset("DT_BasePlayer", "m_fFlags");
	*(int*)(pEntity + offset) = fl;
}
