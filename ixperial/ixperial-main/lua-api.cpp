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
	lua_close(L);
}

void LuaCS::SetupClasses()
{
	getGlobalNamespace(L)
		.beginClass<LuaUCmd>("ucmd")
		.addProperty("buttons", &LuaUCmd::GetButtons, &LuaUCmd::SetButtons)
		.endClass()

		.beginClass<LuaEntity>("entity")
		.addConstructor<void(*)(int), RefCountedPtr<LuaEntity>>()
		.addStaticProperty("localPlayer", &LuaEntity::GetLocalPlayer)
		.addProperty("dormant", &LuaEntity::IsDormant)
		.addProperty("team", &LuaEntity::GetTeam)
		.addProperty("flags", &LuaEntity::GetFlags, &LuaEntity::SetFlags)
		.addProperty("health", &LuaEntity::GetHealth)
		
		.endClass();

	getGlobalNamespace(L)
		.addCFunction("clientcmd", [](lua_State *L)
			-> int {
				luaL_checktype(L, 1, LUA_TSTRING);
				CSGO::GetEngine()->ExecuteClientCmd(lua_tostring(L, 1));
				return 0;
			}
	);

	
		
	// Event system
	/*
	died				entity player
	respawned			entity player
	tookDamage			entity player
	bombPlanted			entity player
	bombBeginDefuse		entity player
	bombEndDefuse		entity player
	chatted				entity player, string message
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

	lua_pushcfunction(L, [](lua_State *L)
		-> int
	{
		luaL_checktype(L, 1, LUA_TSTRING);
		CSGO::GetEngine()->ExecuteClientCmd((std::string("echo ") + lua_tostring(L, 1)).data()); // pretty sure people can escape the str with ';' and execute other cmds
		return 0;
	}
	);
	lua_setglobal(L, "log");

	lua_pushcfunction(L, [](lua_State *L)
		-> int
		{
			luaL_checktype(L, 1, LUA_TNUMBER); // TODO: allow strings too
			lua_pushboolean(L, GetAsyncKeyState(lua_tointeger(L, 1)) & 0x8000);
			return 1;
		}
	);
	lua_setglobal(L, "isKeyDown");
}

void LuaCS::ExecuteAllScripts()
{
	char szPath[MAX_PATH] = { 0 };
	Utility::GetFile("ixperial-main.dll", "scripts", szPath, MAX_PATH);

	std::vector<std::string> files;
	std::string dir = szPath;

	Utility::GetFilesInDirectory(files, dir, 0);

	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++) {
		std::string full_path = dir + "\\" + *it;
		luaL_dofile(L, full_path.data());
	}
}
