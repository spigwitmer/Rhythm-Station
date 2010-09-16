#version 110
uniform sampler2D tex;
uniform float aspect_ratio;

const float blur_factor = 0.0025;
const int samples = 4;

void main()
{
	float total_samples = float(((samples)*2)*(samples*2));
	vec4 texture;
	for(int i = -samples; i < samples; i++)
	{
		for(int j = -samples; j < samples; j++)
		{
			vec2 texCoords = gl_TexCoord[0].st;
			texCoords.y = 1.0-texCoords.y;
			texCoords.x += (float(i) * blur_factor) / aspect_ratio;
			texCoords.y += float(j) * blur_factor;
			texture += texture2D(tex, texCoords) / total_samples;
		}
	}
	gl_FragColor = texture;
}
