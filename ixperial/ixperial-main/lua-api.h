#pragma once

class LuaCS
{
public:
	LuaCS();
	~LuaCS();

	lua_State* L;
};