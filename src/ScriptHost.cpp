#include "ScriptHost.h"

ScriptHost::ScriptHost()
{

}

void ScriptHost::Load()
{
	// Create new lua state and load it
	L = luaL_newstate();

	// load lua libs
	//luaL_openlibs(L);

	// run the lua script
	//luaL_dofile(L, "resources/scripts/HelloWorld.lua");

	// close the lua state
	lua_close(L);
}
