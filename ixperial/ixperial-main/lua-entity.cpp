#include "stdafx.h"

void* LuaEntity::GetEntityFromIndex(int i)
{
	static void *listPtr = (int*)sigScan((int)CSGO::GetClientHandle(), "\xBB\x00\x00\x00\x00\x83\xFF\x01\x0F\x8C\x00\x00\x00\x00\x3B\xF8");

	int list = *(int*)((char*)listPtr + 1);

	return *(int**)(list + (i * 0x10));
}

RefCountedPtr<LuaEntity> LuaEntity::GetLocalPlayer()
{
	static void* lpPtr = (int*)sigScan((int)CSGO::GetClientHandle(), "\xA3\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x59\xC3\x6A\x00");
	int localEntPtr = *(*((int**)((int)lpPtr + 1)) + 4);
	
	LuaEntity *localEnt = new LuaEntity(0);
	localEnt->pEntity = localEntPtr;
	return RefCountedPtr<LuaEntity>(localEnt);
}

void LuaEntity::UpdateEntity() const
{
	if (!pEntity)
	{
		//pEntity = (int)GetEntityFromIndex(idx);
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

RefCountedPtr<LuaVector3> LuaEntity::GetOrigin() const
{
	UpdateEntity(); if (!pEntity) return 0;
	static int offset = g_pVars->GetOffset("DT_BaseEntity", "m_vecOrigin");
	float* pOrigin = (float*)(pEntity + offset); // is this safe? No.
	return RefCountedPtr<LuaVector3>(new LuaVector3(pOrigin[0], pOrigin[1], pOrigin[2]));
}

int LuaEntity::GetShotsFired() const
{
	UpdateEntity(); if (!pEntity) return 0;
	static int offset = g_pVars->GetOffset("DT_CSPlayer", "m_iShotsFired");
	return *(int*)(pEntity + offset);
}

RefCountedPtr<LuaVector3> LuaEntity::GetAimPunch() const
{
	UpdateEntity(); if (!pEntity) return 0;
	static int offset = g_pVars->GetOffset("DT_CSPlayer", "m_aimPunchAngle");
	float* pAimpunch = (float*)(pEntity + offset);
	return RefCountedPtr<LuaVector3>(new LuaVector3(pAimpunch[0], pAimpunch[1], pAimpunch[2]));
}

void LuaEntity::SetFlags(const int& fl)
{
	UpdateEntity(); if (!pEntity) return;
	static int offset = g_pVars->GetOffset("DT_BasePlayer", "m_fFlags");
	*(int*)(pEntity + offset) = fl;
}

