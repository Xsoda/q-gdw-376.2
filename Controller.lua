--[[
Modules:
	- logger
	  log_info(args, str, ...)
	  log_debug(args, str, ...)
	  log_warn(args, str, ...)
	  log_warn(args, str, ...)

--]]

function Dispatch(command, args)
	local wrapper = require("luawrapper")
	wrapper.log_error(4, "come from lua script. command -> ", command, " args -> ", args)
	return 0
end