#pragma once

class LuaEntity
{
public:
	LuaEntity(int i)
	{
		pEntity = (int)GetEntityFromIndex(i);
	}

	mutable int pEntity; // should be void* or Entity* but int is a lot less hassle casting wise

public:
	static void* GetEntityFromIndex(int i);
	static RefCountedPtr<LuaEntity> GetLocalPlayer();

public:
	// Utility
	void UpdateEntity() const;

	// Read properties
	bool IsDormant() const;
	int GetTeam() const;
	int GetFlags() const;
	int GetHealth() const;
	int GetArmor() const;
	RefCountedPtr<LuaVector3> GetOrigin() const;
	int GetShotsFired() const;

	// Write properties
	void SetFlags(const int &fl);
};