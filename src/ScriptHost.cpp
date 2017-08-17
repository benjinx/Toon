#include "ScriptHost.h"

ScriptHost::ScriptHost()
{

}

void ScriptHost::Load()
{
	// Create new lua state and load it
	L = luaL_newstate();

	// load lua libs
	static const luaL_Reg lualibs[] = {
		{ "base", luaopen_base },
		{ NULL, NULL }
	};

	const luaL_Reg* lib = lualibs;

	for (; lib->func != NULL; lib++)
	{
		lib->func(L);
		lua_settop(L, 0);
	}

	// run the lua script
	luaL_dofile(L, "resources/scripts/HelloWorld.lua");

	// close the lua state
	lua_close(L);
}