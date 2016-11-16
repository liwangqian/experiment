
local module_name = 'external'	-- 模块名字，根据需要进行定义，与文件名可不一致
local external = {}

setmetatable(external, {__index = function ( self, name )
	local proxy = {}
	setmetatable(proxy, 
	{
		-- 针对类似devm.CStub:get_something()的调用，生成相应的get_something接口
		__index = function ( self, name )
			self[name] = function ( self, ... )
				print("call proxy["..name.."] with:", ...)
			end
			return self[name]
		end,

		-- 针对devm.get_other()的调用，由于我们返回的是一个表，因此需要定义表的call元方法
		__call = function ( self, ... )
			print("call proxy tb with:", ...)
		end
	})

	-- 这里返回的是一张表，是为了保证external.abc()以及external.class:abc()都能够成功
	external[name] = proxy
	return proxy
end})

_G[module_name] = external
package.loaded[module_name] = external

return external