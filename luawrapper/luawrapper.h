/* -*- encoding: utf-8-unix; -*- */
/* File-name:    <luawrapper.h> */
/* Author:       <小苏打> */
/* Create:       <Thu Oct 25 16:40:25 2012> */
/* Time-stamp:   <Saturday November 24, 9:33:47 2012> */
/* Mail:         <Xsoda@Live.com> */

#ifndef _LUAWRAPPER_H_
#define _LUAWRAPPER_H_

#if defined __cplusplus
extern "C" {
#endif

// include lua file
#include "../lua/lua.h"
#include "../lua/lualib.h"
#include "../lua/lauxlib.h"

// include logger
#include "../logger/logger.h"
#include "../serialport/serialport.h"
#include "../parse/parse.h"
#include "../odbc/odbc.h"

#if defined _DEBUG
#pragma comment(lib, "../Debug/logger.lib")
#pragma comment(lib, "../Debug/lua.lib")
#pragma comment(lib, "../Debug/serialport.lib")
#pragma comment(lib, "../Debug/parse.lib")
#pragma comment(lib, "../Debug/odbc.lib")
#else
#pragma comment(lib, "../Release/logger.lib")
#pragma comment(lib, "../Release/lua.lib")
#pragma comment(lib, "../Release/serialport.lib")
#pragma comment(lib, "../Release/parse.lib")
#pragma comment(lib, "../Release/odbc.lib")
#endif

int luaopen_luawrapper(lua_State *lua);
// int RegisterFunctions(lua_State *lua);
#if defined __cplusplus
}
#endif

#endif /* _LUAWRAPPER_H_ */
