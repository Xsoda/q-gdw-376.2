package.path = package.path .. ";./script/?.lua"
require "logger"

function OpenSerial(arg1, arg2)
	logger.log_info("OpenSerial -> "..arg1.." "..arg2)
	return 1
end

function CloseSerial(arg1, arg2)
	logger.log_info("CloseSerial -> "..arg1.." "..arg2)
	return 1
end

dispatch = {
	open_serial = OpenSerial,
	close_serial = CloseSerial,
}

function Dispatch(command, arg1, arg2)
	logger.log_error("come from lua script. command -> "..command.." arg1 -> "..arg1.." arg2 -> "..arg2)	
	return dispatch[command](arg1, arg2)
end


