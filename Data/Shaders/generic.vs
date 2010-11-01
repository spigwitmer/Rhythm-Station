attribute vec4 VPos;
attribute vec2 VCoords;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

varying vec2 Coords;

void main() {
	Coords = vec2(VCoords);
	gl_Position = ProjectionMatrix * ModelViewMatrix * VPos;
}
