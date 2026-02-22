---@meta lua502-compat

-- Lua 5.0.2 standard library functions not present in Lua 5.1.
-- These stubs prevent LuaLS from flagging valid Lua 5.0.2 code as undefined.
-- Used when editing Dawn of War SCAR scripts (which run on Lua 5.0.2).

---Find all matches of pattern in string (Lua 5.0 name for string.gmatch)
---@param s string
---@param pattern string
---@return fun(): string ...
function string.gfind(s, pattern) end

---Return the size of a table (Lua 5.0; use # operator in 5.1+)
---@param t table
---@return integer
function table.getn(t) end

---Set the size of a table (Lua 5.0 only)
---@param t table
---@param n integer
function table.setn(t, n) end

---Apply function to each key-value pair (Lua 5.0; removed in 5.1)
---@param t table
---@param f fun(key: any, value: any): any?
function table.foreach(t, f) end

---Apply function to each integer-key element (Lua 5.0; removed in 5.1)
---@param t table
---@param f fun(index: integer, value: any): any?
function table.foreachi(t, f) end

---Modulo operation (Lua 5.0 name; renamed to math.fmod in 5.1)
---@param x number
---@param y number
---@return number
function math.mod(x, y) end
