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

sound = LoadSound("get-it-by-your-hands.ogg")
sound:setLoop(true)
sound:UseEqualizer(true)

loc = Object()
loc:setPosition(0,-160,0)

spr = LoadActor("Data/logo.png")
spr:setPosition(0,0,0)

local spacing = 64
local rotations = { 90, 0, 180, -90 }

for i = 1,2 do
	for j = -10,10 do
		spr = LoadActor("Data/arrow.png")
		spr:setPosition(j*spacing, 400, 0)
		spr:setScale(0.5,0.5,0.5)
		spr:addState(0, math.random(1,500)*0.005)
		spr:addState(1, math.random(10,50)*0.1)
		spr:setPosition(j*spacing, -400, 0)
		spr:setLoop(true);
	end
end

--[=[
for i = 0,3 do
	spr = LoadActor("Data/arrow.png")
	spr:setParent(loc)
	spr:setPosition(spacing*(i-1.5),0,0)
	spr:setScale(0.5,0.5,0.5)
	spr:setRotate(0,0,rotations[i+1])
	spr:setParent(loc)
	--[[
	-- animation test
	spr:addState(0, 0.25) -- sleep
	spr:addState(6, 0.5)
	spr:setPosition(spacing*(i-1.5)*2,0,0)
	spr:setScale(0.75,0.75,0.75)
	spr:addState(2,0.5)
	spr:setPosition(spacing*(i-1.5),0,0)
	]]
end
--]=]