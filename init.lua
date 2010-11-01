SLB.using(SLB)

function Trace(...)
	print(TimeStamp() .. " " .. string.format(...))
end

local test_string = string.format("%s @ %ix%i",
	Version, ScreenWidth, ScreenHeight)

Trace(test_string)
Trace("Lua says hi!")


-- note: the order of translate/scale/rotate and loading matters.
-- this will be fixed soon.
ob = Object()
ob:Translate(-160,0,-100)
ob:Scale(0.5,0.5,0.5)
ob:Load("test.png")
ob:Register()

ob = Object()
ob:Translate(160,0,-100)
ob:Scale(0.5,0.5,0.5)
ob:Load("test.png")
ob:Register()