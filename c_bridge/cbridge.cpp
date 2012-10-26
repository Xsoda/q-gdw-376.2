// -*- encoding: utf-8-unix; -*-
// File-name:    <cbridge.cpp>
// Author:       <小苏打>
// Create:       <Thu Oct 25 16:39:42 2012>
// Time-stamp:   <Thursday October 25, 16:39:44 2012>

#include "cbridge.h"

static lua_State *lua;

static int lua_Dispatch(unsigned int command, int arg)
{
    int ret;
    lua_getglobal(lua, "Dispatch");
    lua_pushnumber(lua, command);
    lua_pushnumber(lua, arg);
    if (lua_pcall(lua, 2, 1, 0))
    {
        log_error("lua_Dispatch fail -> %s", lua_tostring(lua, -1));
        return 1;
    }
    else
    {
        ret = lua_tointeger(lua, -1);
        log_info("lua_Dispatch success and return -> %d", ret);
        return ret;
    }
}

int DispatchCommand(unsigned int command, int arg)
{
    log_info("Dispatch command -> %d, arg -> %d", command, arg);
    if (lua_Dispatch(command, arg))
        log_error("DispatchCommand error");
    else
        log_info("DispatchCommand success");
    return 0;
}

int InitializeComponent()
{
    // 初始化 Logger
    Logger_create();
    
    // 初始化LUA
    lua = luaL_newstate();
    luaL_openlibs(lua);
    RegisterFunctions(lua);
    if(luaL_dofile(lua, "Controller.lua"))
        log_error("load file <Controller.lua> fail");
    else
    {
        log_info("load file <Controller.lua> success");
        lua_pcall(lua, 0, 0, 0);
    }
    return 0;
}

int ReleaseComponent()
{
    log_info("release all component");
    log_info("-----------------------------------\n");
    Logger_release();
    lua_close(lua);
    return 0;
}
