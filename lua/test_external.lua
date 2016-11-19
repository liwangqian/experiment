
require("utility")

utility.export_api(safe_require, 'require')	-- 解决external模块找不到的问题

require("external")  -- 模块external未定义

local ext = external_module("external")	-- 定义一个external模块，用于模拟external的行为

print(ext.abc(123))	 -- 可以对ext调用任意函数

print(ext.board:get(123, 'abc', {}))	-- 支持调用ext内部嵌套定义的类board中的任意函数
