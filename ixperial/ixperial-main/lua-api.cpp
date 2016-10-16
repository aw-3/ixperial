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
		.addProperty("team", &LuaEntity::GetTeam)
		.addProperty("flags", &LuaEntity::GetFlags)
		.addProperty("health", &LuaEntity::GetHealth)
		.endClass()

		//.addCFunction("getLocalPlayer", &LuaEntity::GetLocalPlayer)

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
	lua_pushcfunction(L, [](lua_State *L)
		-> int
		{
			luaL_checktype(L, 1, LUA_TSTRING);
			MessageBoxA(0, lua_tostring(L, 1), "Alert", 0);
			return 0;
		}
	);
	lua_setglobal(L, "alert");

	
	
}
