--[[
  - Q/GDW 376.2
    ParseDatagram return's lua table:
	{
		Len : 报文长度
		Dir : 传输方向位
		RouterId : 路由标识
		CommModuleId : 通讯模块标识
		RelayLevel : 中级级别
		ChannelId : 信道标识
		Afn : 应用功能码
		Fn : 数据单元标识
		Data : 应用数据
		DataLen : 应用数据长度
		*** and it's retain ***
		SourceAddr : 源地址
		RelayAddr ： 中继地址
		DestinationAddr ： 目的地址
	}
  - DL/T 645 2007
	ParsePacket return's lua table:
	{
	}
--]]

Datagram = {
	parse = function (t)
		local wrapper = require("luawrapper")
		return wrapper.ParseDatagram(t)
	end,
}

		