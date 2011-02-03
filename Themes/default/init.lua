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

function LoadFont(filename)
	obj = Font()
	obj:Load(filename)
	return obj
end

local vec3 = {
	zero = function()
		return 0, 0, 0
	end
}

sound = LoadSound("get-it-by-your-hands.ogg")
sound:setLoop(true)
sound:UseEqualizer(true)

loc = Object()
loc:setPosition(0,-160,0)

spr = LoadActor("Data/logo.png")
spr:setPosition(vec3.zero())

local size = 0.5
local spacing = 64
local rotations = { 90, 0, 180, -90 }

for i = 0,3 do
	spr = LoadActor("Data/arrow.png")
	spr:setParent(loc)
	spr:setPosition(spacing*(i-1.5),0,0)
	spr:setRotation(0,0,rotations[i+1])
	spr:setScale(size,size,size)
end

for i = 0,3 do
	spr = LoadActor("Data/arrow.png")
	spr:setColor(0,0,0,0.5)
	spr:setRotation(0,0,rotations[i+1])
	spr:setScale(size,size,size)
	spr:setPosition(spacing*(i-1.5),500,0)
	spr:addState(1,5)
	spr:setPosition(spacing*(i-1.5),-500,0)
	spr:setLoop(true)
end
