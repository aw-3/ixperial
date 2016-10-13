#include "stdafx.h"

using namespace luabridge;

LuaCS::LuaCS()
{
	L = luaL_newstate();
	luaL_openlibs(L);

}

LuaCS::~LuaCS()
{

}
