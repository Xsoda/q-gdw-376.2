--[[
Modules:
	- logger
	  log_info(args, str, ...)
	  log_debug(args, str, ...)
	  log_warn(args, str, ...)
	  log_warn(args, str, ...)

--]]

--[[
Command:
	 open serial
	 close serial

--]]
function OpenSerial(args)
	wrapper.log_info(2, "OpenSerial -> ", args)
end

function CloseSerial(args)
	wrapper.log_info(2, "CloseSerial -> ", args)
end

function Dispatch(command, args)
	return 13 + 13
end
--[[
dispatch = {
open_serial = OpenSerial,
close_serial = CloseSerial,
}




function Dispatch(command, args)
	local wrapper = require("luawrapper")
	wrapper.log_error(4, "come from lua script. command -> ", command, " args -> ", args)

	--[[
	for k, v in ipairs(dispatch) do
		wrapper.log_info(4, "find ->", command, "current -> ", k)
		if k == command then
			wrapper.log_info(2, "call function", v)
			v(args)
			return 0
		end
	end
	--]]
	-- dispatch[command](args)
	return 1
end


--]]