local Result = {
    __call = function (self, success, ...)
        self.success = success
        if success then
            self.params = {...}
        else
            self.code = select(1, ...)
            self.desc = select(2, ...)
        end
        return self
    end
}
Result.__index = Result

local function NewResult(success, ...)
    return setmetatable({}, Result)(success, ...)
end

function Success(...)
    return NewResult(true, ...)
end

function Failed(code, desc)
    return NewResult(false, code, desc)
end

function When(fn, ...)
    if fn then
        return fn(...)
    end
    return Failed(-1, 'conditional function is nil')
end

function Result:IsSuccess()
    return self.success
end

function Result:Then(fn, ...)
    if self:IsSuccess() and fn then
        return fn(self, ...)
    end
    return self
end

function Result:Else(fn, ...)
    if not self:IsSuccess() and fn then
        return fn(self, ...)
    end
    return self
end

function Result:Catch(fn, ...)
    if not self:IsSuccess() and fn then
        fn(self, ...)
    end
    return self
end

function Result:Finally(fn, ...)
    if fn then fn(self, ...) end
    return self
end

-- tests

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

print('\n==CASE 1==')

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

print('\n==CASE 2==')

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
