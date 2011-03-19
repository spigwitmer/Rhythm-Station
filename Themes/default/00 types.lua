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
