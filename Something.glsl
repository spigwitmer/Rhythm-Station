-- GL21.Vertex
attribute vec4 Position;
uniform mat4 ModelViewProjection;

void main()
{
	gl_Position = Position;
}

-- GL21.Fragment
void main()
{
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

-- GL32.Vertex
in vec4 Position;
uniform mat4 ModelViewProjection;

void main()
{
	gl_Position = Position;
}

-- GL32.Fragment
out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
