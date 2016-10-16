#include "stdafx.h"

using namespace luabridge;

LuaCS *luacs = nullptr;

std::vector<LuaEventBind*> LuaEvent::binds;

LuaCS::LuaCS()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	SetupClosures();
	SetupClasses();
}

LuaCS::~LuaCS()
{

}

void LuaCS::SetupClasses()
{
	getGlobalNamespace(L).beginNamespace("csgo")
		.beginClass<LuaEntity>("entity")
		.addConstructor<void(*)(int)>()
		.addProperty("dormant", &LuaEntity::IsDormant)
		
		.endClass()
		.endNamespace();
	
	// Event system
	/*
	died				entity player
	respawned			entity player
	tookDamage			entity player
	bombPlanted			entity player
	bombBeginDefuse		entity player
	bombEndDefuse		entity player
	*/

	getGlobalNamespace(L)
		.addCFunction("bindEvent", &LuaEvent::Bind)
		.addCFunction("invokeEvent", &LuaEvent::Invoke)
	.endNamespace();


}

void LuaCS::SetupClosures()
{

}
