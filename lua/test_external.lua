
require("utility")

utility.export_api(safe_require, 'require')  -- 解决external模块找不到的问题

require("external")  -- 模块external未定义

print(ext.abc(123))	 -- 可以对ext调用任意函数

print(ext.board:get(123, 'abc', {}))	-- 支持调用ext内部嵌套定义的类board中的任意函数

--[[
package.loaded['external'] = nil
_G['external'] = nil
--]]

print(pcall(require, "external"))
