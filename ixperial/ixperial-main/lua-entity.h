#pragma once

class LuaEntity
{
public:
	LuaEntity(int i) : idx(i)
	{
		pEntity = (int)GetEntityFromIndex(idx);
	}

	mutable int pEntity; // should be void* or Entity* but int is a lot less hassle casting wise
	int idx;

public:
	static void* GetEntityFromIndex(int i);
	static void* GetLocalPlayer();

public:
	// Utility
	void UpdateEntity() const;

	// Read properties
	bool IsDormant() const;
	int GetTeam() const;
	int GetFlags() const;
	int GetHealth() const;
	int GetArmor() const;
	int ShotsFired() const;
};