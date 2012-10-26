// -*- encoding: utf-8-unix; -*-
// File-name:    <luawrapper.cpp>
// Author:       <小苏打>
// Create:       <Thu Oct 25 16:40:31 2012>
// Time-stamp:   <Thursday October 25, 16:40:33 2012>

#include "luawrapper.h"

// register logger function
static int lua_log_info(lua_State *lua)
{
    const char *content = luaL_checkstring(lua, -1);
    log_info(content);
    return 0;
}

static int lua_log_error(lua_State *lua)
{
    const char *content = luaL_checkstring(lua, -1);
    log_error(content);
    return 0;
}

static int lua_log_debug(lua_State *lua)
{
    const char *content = luaL_checkstring(lua, -1);
    log_debug(content);
    return 0;
}

static int lua_log_warn(lua_State *lua)
{
    const char *content = luaL_checkstring(lua, -1);
    log_warn(content);
    return 0;
}

static const luaL_Reg logger[] = {
    {"log_info", lua_log_info},
    {"log_error", lua_log_error},
    {"log_debuf", lua_log_debug},
    {"log_warn", lua_log_warn},
    {0, 0}
};

int RegisterFunctions(lua_State *lua)
{
    const luaL_Reg *lib = logger;
    for (; lib->func != NULL; lib++)
    {
        lua_pushcfunction(lua, lib->func);
        lua_setglobal(lua, lib->name);
    }
//    luaL_newlibtable(lua, logger);
  //  luaL_setfuncs(lua, logger, 0);
//    lua_setglobal(lua, "logger");
 //   luaL_newlib(lua, logger);
    return 1;
}