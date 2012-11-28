
serialport = {	
	open = function (arg1, arg2)
				local wrapper = require("luawrapper")
				return wrapper.serialport_open(arg1, arg2)
			end,
	close = function ()
				local wrapper = require("luawrapper")
				return wrapper.serialport_close()
			end,
	read = function()
			   local wrapper = require("luawrapper")
			   return wrapper.serialport_read()
			end,
	write = function(data)
				local wrapper = require("luawrapper")
				return wrapper.serialport_write(data)
			end,
	set_received_hook = function (fn)
							local wrapper = require("luawrapper")
							return wrapper.serialport_set_received_callback(fn)
						end,
	set_error_hook = function (fn)
						 local wrapper = require("luawrapper")
						 return wrapper.serialport_set_received_callback(fn)
					end,
	set_pinchange_hook = function (fn)
							 local wrapper = require("luawrapper")
							 return wrapper.serialport_set_pinchange_callback(fn)
						end,
}
