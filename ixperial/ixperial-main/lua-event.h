#pragma once

class LuaEventBind
{
public:
	LuaEventBind(std::string strName, int funcId) : name(strName), funcRef(funcId) {}

	std::string name;
	int funcRef;
};

class LuaEvent
{
public:
	static std::vector<LuaEventBind*> binds;

	static int Bind(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TSTRING);
		luaL_checktype(L, 2, LUA_TFUNCTION);
		lua_checkstack(L, 2);

		int funcId = lua_ref(L, true);

		LuaEventBind *bind = new LuaEventBind(lua_tostring(L, 1), funcId);
		binds.push_back(bind);

		return 0; // TODO: return class where user can call :disconnect to remove it
	}

	static int Invoke(lua_State *L)
	{
		luaL_checktype(L, 1, LUA_TSTRING);

		std::string name = lua_tostring(L, 1);

		for (auto it = binds.begin(); it != binds.end(); it++)
		{
			auto bind = *it;

			if (bind->name == name)
			{
				lua_getref(L, bind->funcRef);
				lua_pcall(L, 0, 0, 0);
			}
		}

		return 0;
	}
};
