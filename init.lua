-- yes, the infamous LoadActor from StepMania.
function LoadActor(filename)
	obj = Object()
	obj:Load(filename)
	return obj
end

function LoadSound(filename)
	obj = Sound()
	obj:Load(filename)
	return obj
end

sound = LoadSound("get-it-by-your-hands.ogg")
sound:Loop(true)
sound:UseEqualizer(true)

loc = Object()
loc:Translate(0,-160,0)

spr = LoadActor("Data/logo.png")
spr:Translate(0,0,0)

local spacing = 64

spr = LoadActor("Data/arrow.png")
spr:Translate(-1.5*spacing,0,0)
spr:Scale(0.5,0.5,0.5)
spr:Rotate(0,0,90)
spr:Parent(loc)

spr = LoadActor("Data/arrow.png")
spr:Translate(-0.5*spacing,0,0)
spr:Scale(0.5,0.5,0.5)
spr:Rotate(0,0,0)
spr:Parent(loc)

spr = LoadActor("Data/arrow.png")
spr:Translate(0.5*spacing,0,0)
spr:Scale(0.5,0.5,0.5)
spr:Rotate(0,0,180)
spr:Parent(loc)

spr = LoadActor("Data/arrow.png")
spr:Translate(1.5*spacing,0,0)
spr:Scale(0.5,0.5,0.5)
spr:Rotate(0,0,-90)
spr:Parent(loc)
