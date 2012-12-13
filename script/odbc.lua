-- odbc wrapper
--[[
lua_odbc_open
lua_odbc_prepare
lua_odbc_exec
lua_odbc_row_count
lua_odbc_column_count
lua_odbc_column_name
lua_odbc_column_text
lua_odbc_column_double
lua_odbc_column_int
lua_odbc_step
lua_odbc_finalize
lua_odbc_close
--]]

Odbc = {}

-- account class methods
Odbc_methods = {
   setDSN = function (self, dsn_)
   rawset(rawget(getmetatable(self), "private"), "dsn", dsn_)
   end,
   open = function (self)
      local wrapper = require("luawrapper")
      local conn = wrapper.lua_odbc_open(rawget(rawget(getmetatable(self), "private"), "dsn"))
	  rawset(rawget(getmetatable(self), "private"), "conn", conn)
   end,
   prepare = function (self, sql)
      local wrapper = require("luawrapper")
      local stmt = wrapper.lua_odbc_prepare(rawget(rawget(getmetatable(self), "private"), "conn"), sql)
	  rawset(rawget(getmetatable(self), "private"), "stmt", stmt)
   end,
   getDSN = function (self)
      return rawget(rawget(getmetatable(self), "private"), "dsn")
   end,
   exec = function (self, sql)
      local wrapper = require("luawrapper")
      wrapper.lua_odbc_exec(rawget(rawget(getmetatable(self), "private"), "conn"), sql)
   end,
   rowcount = function (self)
      local wrapper = require("luawrapper")
      return wrapper.lua_odbc_row_count(rawget(rawget(getmetatable(self), "private"), "stmt"))
   end,
   columncount = function (self)
      local wrapper = require("luawrapper")
      return wrapper.lua_odbc_column_count(rawget(rawget(getmetatable(self), "private"), "stmt"))
   end,
   columnname = function (self, col)
      local wrapper = require("luawrapper")
      return wrapper.lua_odbc_column_count(rawget(rawget(getmetatable(self), "private"), "stmt"), col)
   end,
   columntext = function (self, col)
      local wrapper = require("luawrapper")
      return wrapper.lua_odbc_column_text(rawget(rawget(getmetatable(self), "private"), "stmt"), col)
   end,
   columndouble = function (self, col)
      local wrapper = require("luawrapper")
      return wrapper.lua_odbc_column_double(rawget(rawget(getmetatable(self), "private"), "stmt"), col)
   end,
   columnint = function (self, col)
      local wrapper = require("luawrapper")
      return wrapper.lua_odbc_column_int(rawget(rawget(getmetatable(self), "private"), "stmt"), col)
   end,
   step = function (self)
      local wrapper = require("luawrapper")
      return wrapper.lua_odbc_step(rawget(rawget(getmetatable(self), "private"), "stmt"))
   end,
   finalize = function (self)
      local wrapper = require("luawrapper")
      wrapper.lua_odbc_finalize(rawget(rawget(getmetatable(self), "private"), "stmt"))
   end,
   close = function (self)
      local wrapper = require("luawrapper")
      wrapper.lua_odbc_finalize(rawget(rawget(getmetatable(self), "private"), "conn"))
   end
}
 
function Odbc:new_field(key_, value_)
    print"ERROR: objects of class Account do not accept new fields."
end
 
-- account class destructor
function Odbc:destructor()
    print("~Odbc() called for object ", self)
end

-- account class constructor
function Odbc:constructor(dsn)
    local odbc = {}
    local odbc_mt = {__index = Odbc_methods,
                     __newindex = Odbc.new_field,
                     __gc = Odbc.destructor,
                     private = {}
    }
    setmetatable(odbc, odbc_mt)
    odbc:setDSN(dsn);
    return odbc
end
 
-- metatable to the class (not an object)
setmetatable(Odbc,  {__call = Odbc.constructor})

-- create a new object
-- test = Odbc(<dsn string>) 
-- test = nil
-- collectgarbage()

--[[
function f(y)
    local x = y + 1
    return function() return f(x) end
end
 
collectgarbage('stop')
g = f(0)
for i=1,10000 do
    g = g()
end
 
print(collectgarbage('count'))
collectgarbage('restart')
--]]