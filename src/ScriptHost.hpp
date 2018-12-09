#ifndef SCRIPT_HOST_H
#define SCRIPT_HOST_H

#include <string>
#include <vector>
#include <iostream>

//extern "C" {
//	#include "lua.h"
//	#include "lauxlib.h"
//	#include "lualib.h"
//}

//#include "lua.hpp"

class ScriptHost
{
public:
	ScriptHost();
	virtual ~ScriptHost() = default;

	void Load();

private:
	//lua_State* L;
};

#endif // ! SCRIPT_HOST_H
