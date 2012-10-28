--[[
Modules:
	- logger
	  log_info(str)
	  log_debug(str)
	  log_warn(str)
	  log_warn(str)

--]]
logger = {
	log_error = function (str)
		local wrapper = require("luawrapper")
		wrapper.log_error("<LUA>"..str)
	end,
	log_info = function (str)
		local wrapper = require("luawrapper")
		wrapper.log_info("<LUA>"..str)
	end,
	log_debug = function (str)
		local wrapper = require("luawrapper")
		wrapper.log_debug("<LUA>"..str)
	end,
	log_warn = function (str)
		local wrapper = require("luawrapper")
		wrapper.log_warn("<LUA>"..str)
	end
}

function OpenSerial(arg1, arg2)
	logger.log_info("OpenSerial -> "..arg1.." "..arg2)
end

dispatch = {
open_serial = OpenSerial
}

function Dispatch(command, arg1, arg2)
	logger.log_error("come from lua script. command -> "..command.." arg1 -> "..arg1.." arg2 -> "..arg2)
	dispatch[command](arg1, arg2)
	return 1
end


