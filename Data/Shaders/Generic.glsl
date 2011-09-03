-- GL32.Vertex
in vec4 Position;

uniform mat4 ModelViewProjection;

void main()
{
	gl_Position = ModelViewProjection * Position;
}

--
mandelbrot!
-- GL32.Fragment
out vec4 FragColor;

uniform sampler1D ColorTable;

const float maxIterations = 200.0;
const vec4 insideColor = vec4(vec3(0.0), 1.0);

void main()
{
	vec2 c = vec2(gl_FragCoord.xy / 240.0) - vec2(2.3, 1.0);
	vec2 z = c;

	FragColor = insideColor;

	for (float i = 0.0; i < maxIterations; i += 1.0)
	{
		z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;
		
		if (dot(z, z) > 5.0)
		{
			FragColor = texture(ColorTable, i / maxIterations).arra;
			break;
		}
	}
}

-- dGL32.Fragment
uniform float Time;

out vec4 FragColor;

void main()
{
	FragColor = vec4(cos(Time*0.5), cos(Time*0.5), sin(Time*0.5), 1.0);
}

