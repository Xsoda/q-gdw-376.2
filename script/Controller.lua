package.path = package.path..";./?.lua"
require "logger"

function OpenSerial(arg1, arg2)
	logger.log_info("OpenSerial -> "..arg1.." "..arg2)
end

dispatch = {
open_serial = OpenSerial,
OPEN = OpenSerial
}

function Dispatch(command, arg1, arg2)
	logger.log_error("come from lua script. command -> "..command.." arg1 -> "..arg1.." arg2 -> "..arg2)
	dispatch[command](arg1, arg2)
	return 1
end


