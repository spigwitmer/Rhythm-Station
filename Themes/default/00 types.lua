-- http://www.lua.org/pil/13.4.5.html
function read_only(t)
	local proxy = {}
	
	-- create metatable
	local mt = {
		__index = t,
		__newindex = function(t, k, v)
			error("Attempt to update a read-only table", 2)
		end
	}
	setmetatable(proxy, mt)
	return proxy
end

vec2 = read_only {
	zero = function()
		return 0, 0
	end,
	create = function(input)
		return input, input
	end
}

vec3 = read_only {
	zero = function()
		return 0, 0, 0
	end,
	create = function(input)
		return input, input, input
	end
}

vec4 = read_only {
	zero = function()
		return 0, 0, 0, 0
	end,
	create = function(input)
		return input, input, input, input
	end
}

tween = read_only {
	sleep			= 0, -- basic
	linear			= 1,
	ease_in			= 2, -- ^2
	ease_out		= 3,
	smooth			= 4,
	ease_in_cubic	= 5, -- ^3
	ease_out_cubic	= 6,
	smooth_cubic	= 7
}

align = read_only {
	center	= 0,
	middle	= 0,
	left	= 1,
	top		= 1,
	right	= 2,	
	bottom	= 2
}

local width, height = 854, 480
screen = read_only {
	left	= -(width	* 0.5),
	right	=  (width	* 0.5),
	top		= -(height	* 0.5),
	bottom	=  (height	* 0.5),
	width	= width,
	height	= height
}
