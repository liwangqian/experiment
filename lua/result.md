

# Lua错误处理之Result模式

## 现状

1. 大量的if/else分支进行错误处理，淹没了主体流程，导致函数圈复杂度高，函数难以做到短小

2. 资源申请后，如果存在多个返回点，则需要在每个返回点释放资源，容易因为认为疏忽导致资源在流程返回前未释放，导致资源泄露

   ```lua
   function DoSomeActions()
       local handle = AllocateHandle()
       if handle == nil then
           -- 错误处理
           return ERR
       end
    
       local ret, r1, r2 = DoAction01(...)
       if ret ~= OK then
           -- 错误处理
           ReleaseHandle(handle)
           return ERR
       end
   
       -- 忽略部分代码...
   
       if DoAction02(...) ~= OK then
           -- 错误处理
           ReleaseHandle(handle)
           return ERR
       end
   
       -- 忽略部分代码...
   
       if DoAction03(...) ~= OK then
           -- 错误处理
           ReleaseHandle(handle)
           return ERR
       end
   
       -- 忽略部分代码...
       ReleaseHandle(handle)
       return OK
   end
   ```

## 使用Result后

1. 使用Result重构上述函数

   ```lua
   function NotNil(handle)
       if handle then return Success(handle) end
       return Failed(ERR, 'handle is nil')
   end
   
   function DoAction01()
   	-- ...
       if ret == OK then
           return Success(r1, r2)
       else
   		return Failed(ERR, 'DoAction01 failed')
       end
   end
   
   -- ...
   
   function DoSomeActions()
       local handle = AllocateHandle()
       local result = When(NotNil(handle))
       	:Then(function(rst) return DoAction01() end)
      		:Then(function(rst) return DoAction02() end)
       	:Then(function(rst) return DoAction03() end)
       	:Catch(function(rst) log('error: {}, {}', rst.code, rst.desc) end)
       	:Finally(function(rst) ReleaseHandle(handle) end)
   
       return result:IsSuccess() and OK or ERR
   end
   ```

2. 应用例子1

   ```lua
   local function GetSomethingOk()
       print('Get somehting is ok')
       return Success(1,2,3)
   end
   
   local function GetSomethingFailed()
       print('GetSomethingFailed is called')
       return Failed(-2, 'Get something failed with invalid param')
   end
   
   local function ElseGetOtherthing()
       print('ElseGetOtherthing is called')
       local theThing = 123
       return Success(theThing)
   end
   
   When(GetSomethingOk)
       :Then(function (rst)
           print('then #1 called', rst)
           return Success(3,4,5)
       end)
       :Then(GetSomethingFailed)
       :Then(function (rst)
           print('then #3 will not called', rst)
           return rst
       end)
       :Catch(function (rst)
           print('Catched:', rst.code, rst.desc)
       end)
   
   --[[ output
   Get somehting is ok
   then #1 called	table: 0108A0D0
   GetSomethingFailed is called
   Catched:	-2	Get something failed with invalid param
   --]]
   ```

3. 应用例子2

   ```lua
   When(GetSomethingOk)
       :Then(GetSomethingFailed)
       :Then(function (rst)
           print('will not called', rst)
           return rst
       end)
       :Else(ElseGetOtherthing)
       :Finally(function (rst)
           print('Finally: the thing is', unpack(rst.params))
       end)
   
   --[[ output
   Get somehting is ok
   GetSomethingFailed is called
   ElseGetOtherthing is called
   Finally: the thing is	123
   --]]
   ```

   