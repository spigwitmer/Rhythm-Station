-- GL32.Vertex
in vec4 Position;

uniform mat4 ModelViewProjection;

void main()
{
	gl_Position = ModelViewProjection * Position;
}

-- GL32.Fragment
out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0);
}
