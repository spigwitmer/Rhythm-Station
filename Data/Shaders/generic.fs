uniform sampler2D Texture0;
uniform vec4 Color;

varying vec2 Coords;
varying vec3 Normal;

void main() {
	vec4 texture = texture2D(Texture0, Coords);

	// Fix white edges. Even if the image is pre-multiplied, this should be OK
	texture *= texture.a;
	
	// Prevent optimizing out Normal!
	gl_FragColor = texture * Color + vec4(Normal * 0.0, 0.0);
}
