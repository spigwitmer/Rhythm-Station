#ifndef _MATRIX_UTILS_H_
#define _MATRIX_UTILS_H_

#include "Type.h"

class Matrix {
public:
	Matrix();

	void Load(float m[16]);
	void SetUniform();
	void LoadIdentity();

	// transformations
	void Translate(float x, float y, float z);
	void Rotate(float angle, float x, float y, float z);
	void Scale(float x, float y, float z);

	// projection stuff
	void Perspective(float fov, float aspect, double near, double far);
	void Ortho(float left, float right, float bottom, float top, float near, float far);
	void Ortho(float width, float height, vec2 depth = vec2(-5,5)) {
		this->Ortho(-width/2, width/2, -height/2, height/2, depth.x, depth.y);
	}
	void LookAt(vec3 eye, vec3 center, vec3 up);

	// utils
	void Print();
	void Multiply(float m[16]);

	// vec3 overloads
	void Translate(vec3 pos);
	void Rotate(vec3 rot);
	void Scale(vec3 size);

	// doesn't work for (Matrix*)?
	Matrix& operator * (Matrix &m) { this->Multiply(m.matrix); return *this; }
	Matrix& operator = (Matrix &m)  { memcpy(matrix, m.matrix, 16*sizeof(float)); return *this; }

	float matrix[16];
};

#endif
