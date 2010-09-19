#version 110
uniform sampler2D tex;

void main()
{
	vec2 coords = gl_TexCoord[0].st;
	gl_FragColor = texture2D(tex, coords);
}
