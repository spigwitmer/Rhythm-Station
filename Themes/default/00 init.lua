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
	end,
	create = function(input)
		return input, input, input
	end
}

local vec4 = {
	zero = function()
		return 0, 0, 0, 0
	end
}

local tween = {
	-- basic
	sleep = 0,
	linear = 1,

	-- ^2
	ease_in = 2,
	ease_out = 3,
	smooth = 4,

	-- ^3
	ease_in_cubic = 5,
	ease_out_cubic = 6,
	smooth_cubic = 7
}

sound = LoadSound("Sounds/get-it-by-your-hands.ogg")
--sound:SubscribeTo("SongStarted")
sound:UseEqualizer(true)
sound:setLoop(true)

for i = 1,4 do
	local size = 0.5
	local spacing = 64
	local rotations = { 90, 0, 180, -90 }

	spr = LoadActor("Graphics/arrow.png")
	spr:setPosition(spacing*(i-2.5),-160,0)
	spr:setRotation(0,0,rotations[i])
	spr:setScale(size,size,size)
	spr:setColor(0,0,0,0.5)
end

field = NoteField()
field:setPosition(-32, -160, 1)
field:Load("")

spr = LoadActor("Graphics/logo.png")
spr:addState(tween.sleep, 0.75)
spr:addState(tween.ease_in, 1.5)
spr:setScale(vec3.create(0.35))
spr:setColor(vec4.zero())
