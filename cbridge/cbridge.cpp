// -*- encoding: utf-8-unix; -*-
// File-name:    <cbridge.cpp>
// Author:       <小苏打>
// Create:       <Thu Oct 25 16:39:42 2012>
// Time-stamp:   <Thursday October 25, 16:39:44 2012>

#include "cbridge.h"

static lua_State *lua = 0;

static int lua_Dispatch(char * command, char *arg1, int arg2)
{
    int ret;
    lua_getglobal(lua, "Dispatch");
    lua_pushstring(lua, command);
	if (!arg1)
		lua_pushstring(lua, "(null)");
	else
		lua_pushstring(lua, arg1);
    lua_pushnumber(lua, arg2);
    if (lua_pcall(lua, 3, 1, 0) != LUA_OK)
    {
        return 1;
    }
    else
    {
        ret = (int)lua_tonumber(lua, -1);
        lua_pop(lua, 1);
        return (int)ret;
    }
}

int DispatchCommand(char * command, char *arg1, int arg2)
{
	return lua_Dispatch(command, arg1, arg2);
}

int InitializeComponent()
{
	InitializeSerialPort();
    // 初始化LUA
    lua = luaL_newstate();
    luaL_openlibs(lua);
    if(luaL_dofile(lua, "./script/controller.lua"))
        MessageBoxA(NULL, lua_tostring(lua, -1), "load file <controller.lua> fail", MB_OK);
    return 0;
}

int ReleaseComponent()
{
	ReleaseSerialPort();
    lua_close(lua);
	DispatchCommand("Release", 0, 0);
    return 0;
}
