package.path = package.path .. ";./script/?.lua"
require "logger"
require "serialport"
require "Datagram"

function received_hook()
    data = serialport.read();
    for k,v in pairs(data) do
        logger.log_info("key: "..k.."  | value: "..v)
    end
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
    serialport.received_hook.add(received_hook)
    return serialport.open(arg1, arg2)
end

function CloseSerial(arg1, arg2)
	serialport.received_hook.remove();
    return serialport.close()
end


dispatch = {
    open_serial = OpenSerial,
    close_serial = CloseSerial,
}

function Dispatch(command, arg1, arg2)
	r = Datagram.correct({0x68, 0x15, 0x00, 0x41, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x05, 0x01, 0x00, 0xAB, 0x90, 0x78, 0x56, 0x34, 0x12, 0xC1, 0x16})
	r = Datagram.parse(r);
	logger.log_info(a)
	t = Datagram.show(r)
	logger.log_info(t)
    return dispatch[command](arg1, arg2)
end