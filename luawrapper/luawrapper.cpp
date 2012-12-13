// -*- encoding: utf-8-unix; -*-
// File-name:    <luawrapper.cpp>
// Author:       <小苏打>
// Create:       <Thu Oct 25 16:40:31 2012>
// Time-stamp:   <Saturday November 24, 10:4:12 2012>

#include "luawrapper.h"

typedef struct _ref_list_received{
	int func_ref;
	struct _ref_list_received *next;
} ref_list_received;

static ref_list_received *rl_received = NULL;
static int ref_error, ref_pin;
static lua_State *_lua;
static unsigned char spbuf[1024];
static unsigned char spWrite[1024];
// register logger function
#pragma region logger module wrapper
#ifndef LOG_ONCE
static int lua_log_create(lua_State *lua)
{
	const char *path = luaL_checkstring(lua, -1);
	lua_pushinteger(lua, Logger_create(path));
	return 1;
}
static int lua_log_release(lua_State *lua)
{
	Logger l = luaL_checkinteger(lua, -1);
	Logger_release(l);
	return 0;
}
static int lua_log_info(lua_State *lua)
{
    const char *content = luaL_checkstring(lua, -1);
	Logger l = (Logger)luaL_checkinteger(lua, -2);
    log_info(l, content);
    return 0;
}

static int lua_log_error(lua_State *lua)
{
    const char *content = luaL_checkstring(lua, -1);
	Logger l = (Logger)luaL_checkinteger(lua, -2);
    log_error(l, content);
    return 0;
}

static int lua_log_debug(lua_State *lua)
{
    const char *content = luaL_checkstring(lua, -1);
	Logger l = (Logger)luaL_checkinteger(lua, -2);
    log_debug(l, content);
    return 0;
}

static int lua_log_warn(lua_State *lua)
{
    const char *content = luaL_checkstring(lua, -1);
	Logger l = (Logger)luaL_checkinteger(lua, -2);
    log_warn(l, content);
    return 0;
}
#else
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
#endif
#pragma endregion logger module wrapper

#pragma region help function
/*
 * push the byte array to lua stack, and it key start from 1.
 */
int ByteArray2LuaTable(lua_State *lua, __u8 *data, __u32 len)
{
	lua_newtable(lua);
	for (int i = 1; i <= len; i++)
	{
		lua_pushinteger(lua, i);
		lua_pushinteger(lua, data[i - 1]);
		lua_rawset(lua, -3);
	}
	return 1;
}
/*
 * get byte array in current lua stack.
 */
int LuaTable2ByteArray(lua_State *lua, __u8 *data)
{
	int len = 0, idx;
	idx = lua_gettop(lua);
	lua_pushnil(lua);
	while (lua_next(lua, idx))
	{
		data[len++] = lua_tointeger(lua, -1);
		lua_pop(lua, 1);
	}
	return len;
}
#pragma endregion help function

#pragma region serial port module wrapper
static int baud[] = {300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 33600, 38400, 57600, 115200};
static int lua_serialport_open(lua_State *lua)
{
    CONFIG conf;
    int option = 0;
	int ret;
    TCHAR buf [MAX_PATH];
	const char *port = luaL_checkstring(lua, -2);
	ret = MultiByteToWideChar( CP_ACP, MB_COMPOSITE, port, -1, buf, MAX_PATH);
    int arg = luaL_checkinteger(lua, -1);

	conf.mask = SPCF_PORTNAME;
    conf.pszPortName = buf;

    conf.mask |= SPCF_FLOWCONT;
    option = arg & 0xff;    
    conf.flowControl = (FLOWCONTROL)option;
	
	conf.mask |= SPCF_PARITY;
    option = arg >> 8 & 0xff;    
    conf.bParity = option;

    conf.mask |= SPCF_BAUDRATE;
    conf.dwBaudRate = baud[arg >> 16];

	option = SerialPort_Open(&conf);
    lua_pushnumber(lua, option);
	return 1;
}

static int lua_serialport_close(lua_State *lua)
{
	int ret = 0;
    ret = SerialPort_Close();
	lua_pushnumber(lua, ret);
    return 1;
}

static int lua_serialport_read(lua_State *lua)
{
	int ret;
	int bytes = SerialPort_BytesToRead();
	if (bytes)
	{
		ret = SerialPort_ReadBytes(spbuf, bytes);
		if (ret)
		{
			lua_newtable(lua);
			for (int i = 0; i < ret; i++)
			{
				lua_pushnumber(lua, i + 1); // push index
				lua_pushnumber(lua, spbuf[i]); // push value
				lua_rawset(lua, -3);
			}
		}
		else
			lua_pushnil(lua);
	}
    return 1;
}

static int lua_serialport_write(lua_State *lua)
{
	int len = 0;
	int idx;
	idx = lua_gettop(lua);
	lua_pushnil(lua);
	while (lua_next(lua, idx))
	{
		spWrite[len++] = lua_tointeger(lua, -1); // -1 是 value 的索引， -2 是 key 的索引
		lua_pop(lua, 1);
		log_info("write data %d", spWrite[len - 1]);
	}
	if (len)
	{
		idx = SerialPort_WriteBytes(spWrite, len);
		lua_pushinteger(lua, idx);
	}
	else
		lua_pushnil(lua);
    return 1;
}
static int lua_serialport_set_received_callback(lua_State *lua)
{
	ref_list_received *rlr;
	if ((rlr = (ref_list_received *)malloc(sizeof *rlr)) == NULL)
	{
		log_error("malloc fail in %s:%s:%d", __FILE__, __FUNCTION__, __LINE__);
		return 0;
	}
	rlr->func_ref = luaL_ref(lua, LUA_REGISTRYINDEX);
	if ( rlr->func_ref == LUA_REFNIL )
	{
		log_error("create ref fail in %s:%s:%d", __FILE__, __FUNCTION__, __LINE__);
		free(rlr);
	}
	else
	{
		rlr->next = rl_received;
		rl_received = rlr;
	}
	lua_pop(lua, -1);
    return 1;
}

static int lua_serialport_remove_received_callback(lua_State *lua)
{
	ref_list_received *rlr;
	if (rl_received)
	{
		rlr = rl_received->next;
		free(rl_received);
		rl_received = rlr;
	}
	return 1;
}

static int lua_serialport_set_pinchange_callback(lua_State *lua)
{
	if (ref_pin)
		luaL_unref(lua, LUA_REGISTRYINDEX, ref_pin);
	int ref = luaL_ref(lua, LUA_REGISTRYINDEX);
	ref_pin = ref;
	if ( ref == LUA_REFNIL )
	{
		log_error("create ref fail in lua_serialport_set_pinchange_callback");
	}
	lua_pop(lua, -1);
    return 1;
}

static int lua_serialport_set_error_callback(lua_State *lua)
{
	if (ref_error)
		luaL_unref(lua, LUA_REGISTRYINDEX, ref_error);
	int ref = luaL_ref(lua, LUA_REGISTRYINDEX);
	ref_error = ref;
	if ( ref == LUA_REFNIL )
	{
		log_error("create ref fail in lua_serialport_set_error_callback");
	}
	lua_pop(lua, -1);
	return 1;
}
#pragma endregion serial port module wrapper

#pragma region serial port module callback
int error(char *info)
{
	lua_rawgeti(_lua, LUA_REGISTRYINDEX, ref_error);
	if (lua_isfunction(_lua, -1))
	{
		lua_pushstring(_lua, info);
		if (lua_pcall(_lua, 1, 1, 0) != LUA_OK)
			log_error("callback fail: %s", lua_tostring(_lua, -1));
		else
		{
			log_info("error callback success");
			return lua_tointeger(_lua, -1);
		}		
	}
	return 0;
}
int received()
{
	if (rl_received)
		lua_rawgeti(_lua, LUA_REGISTRYINDEX, rl_received->func_ref);
	if (lua_isfunction(_lua, -1))
		if (lua_pcall(_lua, 0, 1, 0) != LUA_OK)
			log_error("callback fail: %s", lua_tostring(_lua, -1));
		else
		{
			log_info("received callback success");
			return lua_tointeger(_lua, -1);
		}
	return 0;
}
int pin_change(int pin)
{	
	lua_rawgeti(_lua, LUA_REGISTRYINDEX, ref_pin);	
	if (lua_isfunction(_lua, -1))
	{
		lua_pushinteger(_lua, pin);
		if (lua_pcall(_lua, 1, 1, 0) != LUA_OK)
			log_error("callback fail: %s", lua_tostring(_lua, -1));
		else
		{
			log_info("received callback success");
			return lua_tointeger(_lua, -1);
		}	
	}
	return 0;
}
#pragma endregion serial port module callback

#pragma region parse datagram wrapper
static int lua_ParseDatagram(lua_State * lua)
{
	// lua table to bytes
	int len = 0, idx;
	__u8 *data;
	Datagram *dg;
	if ((data = (__u8 *)malloc(1024)) == NULL)
	{
		lua_pushnil(lua);
		return 0;
	}
	len = LuaTable2ByteArray(lua, data);
	if (len)
	{
		dg = ParseDatagram(data, len);
		if (!dg)
		{
			lua_pushnil(lua);
			return 1;
		}

		lua_newtable(lua);

		idx = 0;
		// 构建一个报文错误表，如报文没有错误则为nil
		lua_pushstring(lua, "error");
		if (~dg->packet_status & PACKET_VALID)
		{
			// 报文有错误，只显示错误
			lua_newtable(lua);
			if (~dg->packet_status & PACKET_VALID_START)
			{
				lua_pushinteger(lua, ++idx);
				lua_pushstring(lua, "报文起始字节无效");
				lua_rawset(lua, -3);
			}
			if (~dg->packet_status & PACKET_VALID_LEN)
			{
				lua_pushinteger(lua, ++idx);
				lua_pushstring(lua, "报文长度无效");
				lua_rawset(lua, -3);
			}
			if (~dg->packet_status & PACKET_VALID_CHECKSUM)
			{
				lua_pushinteger(lua, ++idx);
				lua_pushstring(lua, "报文校验和无效");
				lua_rawset(lua, -3);
			}
			if (~dg->packet_status & PACKET_VALID_END)
			{
				lua_pushinteger(lua, ++idx);
				lua_pushstring(lua, "报文终止字节无效");
				lua_rawset(lua, -3);
			}
		}
		else
		{
			lua_pushnil(lua); //没错误，将error table set nil
		}
		free(data);
		lua_rawset(lua, -3);
		if (idx)
			return 1; // 报文有误，下面的不用解析了

		lua_pushstring(lua, "Len");
		lua_pushinteger(lua, dg->len);
		lua_rawset(lua, -3);

		lua_pushstring(lua, "Dir");
		log_info("--");
		lua_pushinteger(lua, dg->prefix->control.DIR);
		lua_rawset(lua, -3);

		lua_pushstring(lua, "RouterId");
		lua_pushinteger(lua, dg->prefix->info.info_down.routerID);
		lua_rawset(lua, -3);

		lua_pushstring(lua, "CommModuleId");
		lua_pushinteger(lua, dg->prefix->info.info_down.commModuleID);
		lua_rawset(lua, -3);

		lua_pushstring(lua, "CommMode");
		lua_pushinteger(lua, dg->prefix->control.commMode);
		lua_rawset(lua, -3);

		lua_pushstring(lua, "RelayLevel");
		lua_pushinteger(lua, dg->prefix->info.info_down.relayLevel);
		lua_rawset(lua, -3);
		
		lua_pushstring(lua, "ChannelId");
		lua_pushinteger(lua, dg->prefix->info.info_down.channelID);
		lua_rawset(lua, -3);


		if (dg->prefix->control.DIR)
		{
			lua_pushstring(lua, "MeterChannelFeature");
			lua_pushinteger(lua, dg->prefix->info.info_up.meterChannelFeature);
			lua_rawset(lua, -3);

			lua_pushstring(lua, "PhaseLineMark");
			lua_pushinteger(lua, dg->prefix->info.info_up.phaseLineMark);
			lua_rawset(lua, -3);
		}

		lua_pushstring(lua, "Afn");
		lua_pushinteger(lua, dg->infix->AFN);
		lua_rawset(lua, -3);

		lua_pushstring(lua, "Fn");
		lua_pushinteger(lua, GetFn(dg->infix->DT1, dg->infix->DT2));
		lua_rawset(lua, -3);

		lua_pushstring(lua, "DataLen");
		lua_pushinteger(lua, dg->userDataLen);
		lua_rawset(lua, -3);

		lua_pushstring(lua, "Data");
		if (dg->userDataLen > 0)
			ByteArray2LuaTable(lua, dg->userData, dg->userDataLen);
		else
			lua_pushnil(lua);
		lua_rawset(lua, -3);

		if (dg->prefix->info.info_down.commModuleID)
		{
			if (dg->packet_addr.sourceAddr)
			{
				lua_pushstring(lua, "SourceAddr");
				ByteArray2LuaTable(lua, dg->packet_addr.sourceAddr, 6);
				lua_rawset(lua, -3);
			}
			if (dg->packet_addr.destinationAddr)
			{
				lua_pushstring(lua, "SourceAddr");
				ByteArray2LuaTable(lua, dg->packet_addr.destinationAddr, 6);
				lua_rawset(lua, -3);
			}
			if (dg->packet_addr.relayAddr && dg->prefix->info.info_down.relayLevel)
			{
				lua_pushstring(lua, "RelayAddr");
				lua_newtable(lua);
				for (int i = 1; i <= dg->prefix->info.info_down.relayLevel; i++)
				{
					lua_pushinteger(lua, i);
					ByteArray2LuaTable(lua, &dg->packet_addr.relayAddr[(i - 1) * 6], 6);
					lua_rawset(lua, -3);
				}
				lua_rawset(lua, -3);
			}
		}
	}
    return 1;
}

static int lua_CorrectDatagram(lua_State *lua)
{
	__u8 *data;
	__u16 len = 0;
	if ((data = (__u8 *)malloc(sizeof __u8 * 1024)) == NULL)
	{
		lua_pushnil(lua);
		return 0;
	}
	len = LuaTable2ByteArray(lua, data);
	for (int i = 0; i < len; i++)
		log_info("0x%02X", data[i]);
	len = (__u16)CorrectDatagram(data, len);
	log_info("------------------------------------");
	for (int i = 0; i < len; i++)
		log_info("0x%02X", data[i]);
	ByteArray2LuaTable(lua, data, (__u32)len);
	free(data);
	return 1;
}
static int lua_ParsePacket(lua_State *lua)
{
	return 1;
}
#pragma endregion parse datagram wrapper

static const luaL_Reg logger[] = {
    // logger.dll
#ifndef LOG_ONCE
	{"log_create", lua_log_create},
	{"log_create", lua_log_release},
#endif
    {"log_info", lua_log_info},
    {"log_error", lua_log_error},
    {"log_debuf", lua_log_debug},
    {"log_warn", lua_log_warn},
    {"serialport_open", lua_serialport_open},
    {"serialport_close", lua_serialport_close},
	{"serialport_read", lua_serialport_read},
	{"serialport_write", lua_serialport_write},
    {"serialport_set_received_callback", lua_serialport_set_received_callback},
	{"serialport_remove_received_callback", lua_serialport_remove_received_callback},
    {"serialport_set_pinchange_callback", lua_serialport_set_pinchange_callback},
    {"serialport_set_error_callback", lua_serialport_set_error_callback},
	{"ParseDatagram", lua_ParseDatagram},
	{"ParsePacket", lua_ParsePacket},
	{"CorrectDatagram", lua_CorrectDatagram},
    {0, 0}
};

int luaopen_luawrapper(lua_State *lua)
{
	_lua = lua;
	SetSerialPortCallback(error, received, pin_change);
    luaL_newlibtable(lua, logger);
    luaL_setfuncs(lua, logger, 0);
    return 1;
}
#if 0
int RegisterFunctions(lua_State *lua)
{
    const luaL_Reg *lib = logger;
    for (; lib->func != NULL; lib++)
    {
        lua_pushcfunction(lua, lib->func);
        lua_setglobal(lua, lib->name);
    }
    return 1;
}
#endif

