function Trace(...)
	print(TimeStamp() .. " " .. string.format(...))
end

function LoadActor(filename)
	obj = Object()
	obj:Load(filename)
	obj:Register()
	return obj
end

--[[
function LoadSound(filename)
	snd = Sound()
	snd:Load(filename)
	snd:Register()
	return snd
end
--]]

local test_string = string.format("%s @ %ix%i",
	Version, ScreenWidth, ScreenHeight)

Trace(test_string)
Trace("Lua says hi!")

-- note: the order of translate/scale/rotate and loading matters.
-- this will be fixed soon.

-- yes, the infamous LoadActor from StepMania.
spr = LoadActor("test.png")
spr:Translate(-160,0,0)
spr:Scale(0.5,0.5,0.5)

spr = LoadActor("test.png")
spr:Translate(160,0,0)
spr:Scale(0.5,0.5,0.5)

--[[
sound = LoadSound("test.ogg")
sound:Play()
--]]