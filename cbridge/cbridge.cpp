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
    lua_pushstring(lua, arg1);
    lua_pushnumber(lua, arg2);
    if (lua_pcall(lua, 3, 1, 0) != LUA_OK)
    {
        log_error("lua_Dispatch fail -> %s", lua_tostring(lua, -1));
        return 1;
    }
    else
    {
        if (!lua_isnumber(lua, -1))
            log_error("Dispatch function must return a number");        
        ret = lua_tonumber(lua, -1);
        log_info("lua_Dispatch success -> %d", ret);
        lua_pop(lua, 1);
        return ret;
    }
}

int DispatchCommand(char * command, char *arg1, int arg2)
{
    log_info("Dispatch command -> %s, arg1 -> %s, arg2 -> %d", command, arg1, arg2);
    if (lua_Dispatch(command, arg1, arg2))
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
    // RegisterFunctions(lua);
    if(luaL_dofile(lua, "Controller.lua"))
        log_error("load file <Controller.lua> fail :: %s", lua_tostring(lua, -1));
    else
    {
        log_info("load file <Controller.lua> success");       
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
