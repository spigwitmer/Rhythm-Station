#version 110
uniform sampler2D Texture0;

const int 6;
const int samples2 = num_samples / 2;

void main()
{
	vec4 texture;
	for(int i = -samples2; i < samples2; i++)
	{
		for(int j = -samples2; j < samples2; j++)
		{
			vec2 texCoords = gl_TexCoord[0].st;
			texCoords.x += float(i) * 0.0025;
			texCoords.y += float(j) * 0.0025;
			texture += texture2D(Texture0, texCoords) / pow(float(num_samples),2.0);
		}
	}
	gl_FragColor = texture;
}
