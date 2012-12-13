package.path = package.path .. ";./script/?.lua"
require "logger"
require "serialport"
require "Datagram"
require "odbc"

logger = Logger("../log.txt")
logger:info("Test Logger Module")

function received_hook()
    data = serialport.read();
    for k,v in pairs(data) do
        logger:info("key: "..k.."  | value: "..v)
    end
end

function error_hook(info)
    logger:info("serial error: "..info)
end

function pinchange_hook(pin)
    logger:info("serial port pin changed: ".. pin)
end

function OpenSerial(arg1, arg2)
    serialport.set_error_hook(error_hook)
    serialport.set_pinchange_hook(pinchange_hook)
    serialport.received_hook.add(received_hook)
    return serialport.open(arg1, arg2)
end

function CloseSerial(arg1, arg2)
	serialport.received_hook.remove();
    return serialport.close()
end

function ReleaseAllComponent(arg1, arg2)
	logger = nil;
end

dispatch = {
    open_serial = OpenSerial,
    close_serial = CloseSerial,
	Release = ReleaseAllComponent,
}

function Dispatch(command, arg1, arg2)
	logger:info("Dispatch "..arg1.." "..arg2)	
	database = Odbc("DRIVER={Microsoft Excel Driver (*.xls)};DBQ=demo.xls")
	database:open()
	database:exec("delete from Sheet3 where code=9;")
	database:close()
    return dispatch[command](arg1, arg2)
end
