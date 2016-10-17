#pragma once

#include "lua-event.h"
#include "lua-ucmd.h"
#include "lua-entity.h"

class LuaCS
{
public:
	LuaCS();
	~LuaCS();

	void ExecuteAllScripts();

	lua_State* L;

private:
	void SetupClasses();
	void SetupClosures();	
};

extern LuaCS *luacs;