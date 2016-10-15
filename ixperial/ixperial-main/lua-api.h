#pragma once

#include "lua-event.h"
#include "lua-entity.h"

class LuaCS
{
public:
	LuaCS();
	~LuaCS();

	lua_State* L;

private:
	void SetupClasses();
};

extern LuaCS *luacs;