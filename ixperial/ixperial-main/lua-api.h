#pragma once

#include "lua-vector.h"
#include "lua-event.h"
#include "lua-guiobject.h"
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
	void Initialize();
	void SetupClasses();
	void SetupClosures();
	void SetupGlobalConstants();
};

extern LuaCS *luacs;