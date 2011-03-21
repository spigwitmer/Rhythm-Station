vec3 = {
	zero = function()
		return 0, 0, 0
	end,
	create = function(input)
		return input, input, input
	end
}

vec4 = {
	zero = function()
		return 0, 0, 0, 0
	end,
	create = function(input)
		return input, input, input, input
	end
}

tween = {
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

align = {
	center = 0,
	middle = 0,

	left = 1,
	top = 1,

	right = 2,	
	bottom = 2
}

screen = {
	left = -(854/2),
	right = 854/2,
	top = -(480/2),
	bottom = 480/2,
	width = 854,
	height = 480
}
