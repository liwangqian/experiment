
require("utility")

local module_name = 'external'	-- 模块名字，根据需要进行定义，与文件名可不一致
local external = {}

utility.add_dynamic_binding(external, module_name)
utility.export_module(external, module_name)

return external