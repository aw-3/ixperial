#include "stdafx.h"

using namespace luabridge;

LuaCS *luacs = nullptr;

std::vector<LuaEventBind*> LuaEvent::binds;

LuaCS::LuaCS()
{
	Initialize();
}

LuaCS::~LuaCS()
{
	lua_close(L);
	L = nullptr;
}

void LuaCS::Initialize()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	SetupClosures();
	SetupClasses();
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
		.addProperty("origin", &LuaEntity::GetOrigin)
		
		.endClass();

	getGlobalNamespace(L)
		.addCFunction("clientcmd", [](lua_State *L)
			-> int {
				luaL_checktype(L, 1, LUA_TSTRING);
				CSGO::GetEngine()->ExecuteClientCmd(lua_tostring(L, 1));
				return 0;
			}
	);

	getGlobalNamespace(L)
	.beginClass<LuaVector2>("vector2")
		.addConstructor<void(*)(float, float), RefCountedPtr<LuaVector2>>()
		.addData("x", &LuaVector2::x)
		.addData("y", &LuaVector2::y)
	.endClass()
	.beginClass<LuaVector3>("vector3")
		.addConstructor<void(*)(float, float, float), RefCountedPtr<LuaVector3>>()
		.addData("x", &LuaVector3::x)
		.addData("y", &LuaVector3::y)
		.addData("z", &LuaVector3::z)
	.endClass();
	
		
	// Event system
	/*
	createmove			ucmd
	died				entity
	respawned			entity
	tookDamage			entity
	bombPlanted			entity
	bombBeginDefuse		entity
	bombEndDefuse		entity
	chatted				entity, string message
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
			lua_pushboolean(L, GetAsyncKeyState(lua_tointeger(L, 1)));
			return 1;
		}
	);
	lua_setglobal(L, "isKeyDown");
}

void LuaCS::SetupGlobalConstants()
{
	// IN_BUTTONS

	lua_pushinteger(L, 1);
	lua_setglobal(L, "IN_ATTACK");

	lua_pushinteger(L, 2);
	lua_setglobal(L, "IN_JUMP");

	lua_pushinteger(L, 4);
	lua_setglobal(L, "IN_DUCK");

	lua_pushinteger(L, 8);
	lua_setglobal(L, "IN_FORWARD");

	lua_pushinteger(L, 16);
	lua_setglobal(L, "IN_BACK");

	lua_pushinteger(L, 32);
	lua_setglobal(L, "IN_USE");

	lua_pushinteger(L, 64);
	lua_setglobal(L, "IN_CANCEL");

	lua_pushinteger(L, 128);
	lua_setglobal(L, "IN_LEFT");

	lua_pushinteger(L, 256);
	lua_setglobal(L, "IN_RIGHT");

	lua_pushinteger(L, 512);
	lua_setglobal(L, "IN_MOVELEFT");

	lua_pushinteger(L, 1024);
	lua_setglobal(L, "IN_MOVERIGHT");

	lua_pushinteger(L, 2048);
	lua_setglobal(L, "IN_ATTACK2");


	// fFlags


	// These are all fucking wrong
	// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/const.h
	/*
	lua_pushinteger(L, 1);
	lua_setglobal(L, "FL_ONGROUND");

	lua_pushinteger(L, 2);
	lua_setglobal(L, "FL_DUCKING");

	lua_pushinteger(L, 1024);
	lua_setglobal(L, "FL_FLY");

	lua_pushinteger(L, 8192);
	lua_setglobal(L, "FL_NPC");

	lua_pushinteger(L, 16384);
	lua_setglobal(L, "FL_GODMODE");
	*/
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
