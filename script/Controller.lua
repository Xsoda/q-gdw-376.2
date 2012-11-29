package.path = package.path .. ";./script/?.lua"
require "logger"
require "serialport"
require "parse"

function received_hook()
    logger.log_info("received data")
    data = serialport.read();
    for k,v in pairs(data) do
        logger.log_info("key: "..k.."  | value: "..v)
    end
    serialport.write({0, 1, 2, 3, 4, 5})
end

function error_hook(info)
    logger.log_info("serial error: "..info)
end

function pinchange_hook(pin)
    logger.log_info("serial port pin changed: ".. pin)
end

function OpenSerial(arg1, arg2)
	local a = 0
    serialport.set_error_hook(error_hook)
    serialport.set_pinchange_hook(pinchange_hook)
    serialport.set_received_hook(received_hook)
    return serialport.open(arg1, arg2)
end

function CloseSerial(arg1, arg2)
    return serialport.close()
end


dispatch = {
    open_serial = OpenSerial,
    close_serial = CloseSerial,
}

function ShowDatagram (r)
	logger.log_info("Len: "..r.Len)
	logger.log_info("Dir: "..r.Dir)
	logger.log_info("RouterId: "..r.RouterId)
	logger.log_info("CommModuleId: "..r.CommModuleId)
	logger.log_info("RelayLevel: "..r.RelayLevel)
	logger.log_info("ChannelId: "..r.ChannelId)
	logger.log_info("Afn: "..r.Afn)
	logger.log_info("Fn: "..r.Fn)
	logger.log_info("DataLen: "..r.DataLen)
	s = ""
	for _, v in pairs(r.Data) do
		s = s..string.format(" 0x%x", v)
	end
	logger.log_info("Data:"..s)
end

function Dispatch(command, arg1, arg2)
    logger.log_error("come from lua script. command -> "..command.." arg1 -> "..arg1.." arg2 -> "..arg2)
	p = {0x68, 0x15, 0x00, 0x41, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x05, 0x01, 0x00, 0xAB, 0x90, 0x78, 0x56, 0x34, 0x12, 0xC8, 0x16}
	r = Datagram.parse(p)
	ShowDatagram(r)
    return dispatch[command](arg1, arg2)
end


