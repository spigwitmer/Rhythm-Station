-- yes, the infamous LoadActor from StepMania.
function LoadActor(filename)
	obj = Sprite()
	obj:Load(filename)
	return obj
end

function LoadSound(filename)
	obj = Sound()
	obj:Load(filename)
	return obj
end

sound = LoadSound("get-it-by-your-hands.ogg")
sound:setLoop(true)
sound:UseEqualizer(true)

loc = Object()
loc:setPosition(0,-160,0)

spr = LoadActor("Data/logo.png")
spr:setPosition(0,0,0)

local spacing = 64
local rotations = { 90, 0, 180, -90 }

for i = 0,3 do
	spr = LoadActor("Data/arrow.png")
	spr:setParent(loc)
	spr:setPosition(spacing*(i-1.5),0,0)
	spr:setScale(0.5,0.5,0.5)
	spr:setRotate(0,0,rotations[i+1])
	spr:setParent(loc)
	spr:addState(0, 0.25) -- sleep
	spr:addState(6, 0.5)
	spr:setPosition(spacing*(i-1.5)*2,0,0)
	spr:setScale(0.75,0.75,0.75)
	spr:addState(2,0.5)
	spr:setPosition(spacing*(i-1.5),0,0)
end
