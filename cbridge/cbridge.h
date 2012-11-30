/* -*- encoding: utf-8-unix; -*- */
/* File-name:    <cbridge.h> */
/* Author:       <Ð¡ËÕ´ò> */
/* Create:       <Thu Oct 25 16:39:49 2012> */
/* Time-stamp:   <Thursday October 25, 16:43:28 2012> */
/* Mail:         <Xsoda@Live.com> */

#ifndef _CBRIDGE_H_
#define _CBRIDGE_H_

#if defined __cplusplus
extern "C" {
#endif

// include lua file
#include "../lua/lua.h"
#include "../lua/lualib.h"
#include "../lua/lauxlib.h"

// include logger file
#include "../logger/logger.h"
#include "../luawrapper/luawrapper.h"
#include "../serialport/serialport.h"


#if defined _DEBUG
#pragma comment(lib, "../Debug/lua.lib")
#pragma comment(lib, "../Debug/logger.lib")
#pragma comment(lib, "../Debug/luawrapper.lib")
#pragma comment(lib, "../Debug/serialport.lib")
#else
#pragma comment(lib, "../Release/lua.lib")
#pragma comment(lib, "../Release/logger.lib")
#pragma comment(lib, "../Release/luawrapper.lib")
#pragma comment(lib, "../Release/serialport.lib")
#endif

int DispatchCommand(char * command, int arg);
int InitializeComponent();
int ReleaseComponent();

#if defined __cplusplus
}
#endif

#endif /* _CBRIDGE_H_ */
