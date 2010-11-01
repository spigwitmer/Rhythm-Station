attribute vec4 VPos;
attribute vec4 VNor;
attribute vec2 VCoords;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

varying vec2 Coords;
varying vec3 Normal;

void main() {
	Coords = vec2(VCoords);
	Normal = vec3(VNor);
	gl_Position = ProjectionMatrix * ModelViewMatrix * VPos;
}
