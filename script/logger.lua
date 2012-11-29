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
