package.path = package.path .. ";./script/?.lua"
require "logger"
require "serialport"

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
    logger.log_info("OpenSerial -> "..arg1.." "..arg2)
    serialport.set_error_hook(error_hook)
    serialport.set_pinchange_hook(pinchange_hook)
    serialport.set_received_hook(received_hook)
    return serialport.open(arg1, arg2)
end

function CloseSerial(arg1, arg2)
    logger.log_info("CloseSerial -> "..arg1.." "..arg2)
    return serialport.close()
end


dispatch = {
    open_serial = OpenSerial,
    close_serial = CloseSerial,
}

function Dispatch(command, arg1, arg2)
    logger.log_error("come from lua script. command -> "..command.." arg1 -> "..arg1.." arg2 -> "..arg2)    
    return dispatch[command](arg1, arg2)
end

