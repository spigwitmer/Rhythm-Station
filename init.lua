function Trace(...)
	print(TimeStamp() .. " " .. string.format(...))
end

function LoadActor(filename)
	obj = Object()
	obj:Load(filename)
	obj:Register()
	return obj
end

function LoadSound(filename)
	obj = Sound()
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

local spacing = 64

-- yes, the infamous LoadActor from StepMania.
spr = LoadActor("Data/arrow.png")
spr:Translate(-1.5*spacing,0,0)
spr:Scale(0.5,0.5,0.5)
spr:Rotate(0,0,90)
spr:Color(0.5,0.5,0.5,1.0)

spr = LoadActor("Data/arrow.png")
spr:Translate(-0.5*spacing,0,0)
spr:Scale(0.5,0.5,0.5)
spr:Rotate(0,0,0)
spr:Color(0.5,0.5,0.5,1.0)

spr = LoadActor("Data/arrow.png")
spr:Translate(0.5*spacing,0,0)
spr:Scale(0.5,0.5,0.5)
spr:Rotate(0,0,180)
spr:Color(0.5,0.5,0.5,1.0)

spr = LoadActor("Data/arrow.png")
spr:Translate(1.5*spacing,0,0)
spr:Scale(0.5,0.5,0.5)
spr:Rotate(0,0,-90)
spr:Color(0.5,0.5,0.5,1.0)


sound = LoadSound("shield-9.ogg")
sound:Loop(true)