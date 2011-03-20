-- here until I fix things with Lua
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

local align = {
	center = 0,
	middle = 0,
	
	left = 1,
	top = 1,
	
	right = 2,	
	bottom = 2
}

local screen = {
	left = -(854/2),
	right = 854/2,
	top = -(480/2),
	bottom = 480/2,
	width = 854,
	height = 480
}

sound = LoadSound("Sounds/get-it-by-your-hands.ogg")
--sound:SubscribeTo("SongStarted")
sound:UseEqualizer(true)
sound:setLoop(true)

local loc = Object()
loc:setPosition(0, -130, 0)

for i = 1,4 do
	local size = 0.5
	local spacing = 64
	local rotations = { 90, 0, 180, -90 }

	ob = LoadActor("Graphics/arrow.png")
	ob:setParent(loc)
	ob:setPosition(spacing*(i-2.5),0,0)
	ob:setRotation(0,0,rotations[i])
	ob:setScale(size,size,size)
	ob:setColor(0,0,0,0.5)
end

field = NoteField()
field:setPosition(-32, 0, 0)
field:setParent(loc)
field:Load("")

ob = LoadActor("Graphics/logo.png")
ob:setPosition(0,0,0)
ob:addState(tween.sleep, 0.75)
ob:addState(tween.ease_in, 1.5)
ob:setScale(vec3.create(0.35))
ob:setColor(vec4.zero())

-- themin'
ob = LoadActor("Graphics/frame-top-bg.png")
ob:setSize(screen.width, 62, 1)
ob:setAlign(0,1)
ob:setPosition(0, screen.top, 0)


ob = LoadActor("Graphics/frame-top-clock.png")
ob:setAlign(align.left, align.top)
ob:setPosition(screen.left-ob:getWidth(), screen.top+7, 0)
ob:setColor(1,1,1,0)
ob:addState(tween.sleep, 0.25)
ob:addState(tween.ease_out, 1)
ob:setPosition(screen.left+7,screen.top+7,0)
ob:setColor(1,1,1,1)

ob = LoadActor("Graphics/frame-bottom-bg.png")
ob:setSize(screen.width, ob:getHeight()*78, 0)
ob:setAlign(0,align.bottom)
ob:setPosition(0, screen.bottom, 0)

ob = LoadActor("Graphics/frame-bottom-cover.png")
ob:setAlign(1,2)
ob:setPosition(screen.left-ob:getWidth(), screen.bottom-18, 0)
ob:addState(tween.sleep, 0.25)
ob:addState(tween.east_out, 1.0)
ob:setPosition(screen.left, screen.bottom-18, 0)

ob = LoadActor("Graphics/frame-bottom-cover.png")
ob:setAlign(1,2)
ob:setSize(-272, 58, 1)
ob:setPosition(screen.right, screen.bottom-18, 0)
ob:addState(tween.sleep, 0.25)
ob:addState(tween.east_out, 1.0)
ob:setPosition(screen.right, screen.bottom-18, 0)
