uniform sampler2D Texture0;
uniform vec4 Color;

varying vec2 Coords;
varying vec3 Normal;

void main() {
	vec4 texture = texture2D(Texture0, Coords);
	gl_FragColor = texture * Color + vec4(Normal * 0.0, 0.0);
}
