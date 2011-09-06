-- Vertex.GL32
in vec4 Position;

uniform mat4 ModelViewProjection;

void main()
{
	gl_Position = ModelViewProjection * Position;
}

-- Fragment.GL32
out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0);
}
