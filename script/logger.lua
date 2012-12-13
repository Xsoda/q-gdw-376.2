--[[
Modules:
    - logger
	log_create
	log_release
      log_info(str)
      log_debug(str)
      log_warn(str)
      log_warn(str)

--]]

Logger = {}

Logger_methods = {
    error = function (slef, str)
        local wrapper = require("luawrapper")
		if str then
			wrapper.log_error(rawget(rawget(getmetatable(self), "private"), "logger"), "<LUA>"..str)
		end
    end,
    info = function (self, str)
        local wrapper = require("luawrapper")
		if str then
			wrapper.log_info(rawget(rawget(getmetatable(self), "private"), "logger"), "<LUA>"..str)
		end
    end,
    debug = function (str)
        local wrapper = require("luawrapper")
		if str then
			wrapper.log_debug(rawget(rawget(getmetatable(self), "private"), "logger"), "<LUA>"..str)
		end
    end,
    warn = function (self, str)
        local wrapper = require("luawrapper")
		if str then
			wrapper.log_warn(rawget(rawget(getmetatable(self), "private"), "logger"), "<LUA>"..str)
		end
    end,
	setOutput = function (self, path)
		rawset(rawget(getmetatable(self), "private"), "device", path)
	end,
	create = function (self)
		local wrapper = require("luawrapper")
		local l = wrapper.log_create(rawget(rawget(getmetatable(self), "private"), "device"))
		rawset(rawget(getmetatable(self), "private"), "logger", l)
	end
}

function Logger:new_field(key_, value_)

end
 
-- account class destructor
function Logger:destructor()
	local wrapper = require("luawrapper")
	wrapper.log_release(rawget(rawget(getmetatable(self), "private"), "logger"))
	rawset(rawget(getmetatable(self), "private"), nil)
	setmetatale(self, nil)
end

-- Logger class constructor
function Logger:constructor(path)
    local logger = {}
    local logger_mt = {__index = Logger_methods,
                     __newindex = Logger.new_field,
                     __gc = Logger.destructor,
                     private = {}
    }
    setmetatable(logger, logger_mt)
    logger:setOutput(path)
	logger:create()
    return logger
end

setmetatable(Logger,  {__call = Logger.constructor})