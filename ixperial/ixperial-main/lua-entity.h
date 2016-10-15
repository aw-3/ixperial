#pragma once

class LuaEntity
{
public:
	LuaEntity(int idx)
	{
		MessageBoxA(0, "constructor called", std::to_string(idx).c_str(), 0);
	}

	// Impl methods

};