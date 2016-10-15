#include "stdafx.h"

using namespace luabridge;

LuaCS *luacs = nullptr;

LuaCS::LuaCS()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	SetupClasses();

	luaL_dostring(L, "local ent = csgo.entity(3);");
}

LuaCS::~LuaCS()
{

}

void LuaCS::SetupClasses()
{
	getGlobalNamespace(L).beginNamespace("csgo")
		.beginClass<LuaEntity>("entity")
		.addConstructor<void(*)(int)>()
		// .addProperty("pls", LuaEntity::getOffset, LuaEntity::setOffset);
		
		.endClass()
		.endNamespace();
	
	// Event system
	getGlobalNamespace().beginNamespace("events")
		.addCFunction("bind", (lua_CFunction)LuaEvent::Bind)
		.endNamespace();
}
