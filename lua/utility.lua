module ("utility", package.seeall)

--[[
	向mod模块中增加动态绑定功能
--]]
function add_dynamic_binding(mod, module_name)
	setmetatable(mod, 
	{
		__index = function ( self, name )
			local proxy = {}
			setmetatable(proxy, 
			{
				-- 针对类似external.class:abc()的调用，生成相应的abc接口
				__index = function ( self, name_ )
					self[name_] = function ( self, ... )
						local msg = string.format("call %s.%s:%s with:", module_name, name, name_)
						print(msg, ...)
					end
					return self[name_]
				end,

				-- 针对devm.abc()的调用，由于我们返回的是一个表，因此需要定义表的call元方法
				__call = function ( self, ... )
					print("call "..module_name.."."..name.." with:", ...)
				end
			})

			-- 这里返回的是一张表，是为了保证external.abc()以及external.class:abc()都能够成功
			mod[name] = proxy
			return proxy
		end
	})
end

function export_module(mod, module_name)
	_G[module_name] = mod
	package.loaded[module_name] = mod
end

function export_api(callable, name)
	_G[name] = callable
end