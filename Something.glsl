-- GL21.Vertex
oh
-- GL21.Fragment
hi
-- GL32.Vertex
in vec4 Position;
uniform mat4 ModelViewProjection;

void main()
{
	gl_Position = ModelViewProjection * Position;
}

-- GL32.Fragment
out vec4 color;

void main()
{
	color = vec4(1.0, 0.0, 0.0, 1.0);
}
