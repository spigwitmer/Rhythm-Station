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
