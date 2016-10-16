#pragma once

class LuaEntity
{
public:
	LuaEntity(int i) : idx(i)
	{
		
	}

	int idx;

public:
	// Read-only properties
	bool IsDormant() const;
	int GetTeam();
	int GetFlags();
	int GetHealth();
	int GetArmor();
	int ShotsFired();
};