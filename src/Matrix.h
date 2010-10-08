#ifndef _MATRIX_UTILS_H_
#define _MATRIX_UTILS_H_

#include "Type.h"

enum
{
	STACK_NORMAL = 0,
	STACK_UNDERFLOW,
	STACK_OVERFLOW
};

enum MatrixType
{
	PROJECTION_MATRIX = 0,
	MODELVIEW_MATRIX,
	TEXTURE_MATRIX0,
	TEXTURE_MATRIX1,
	TEXTURE_MATRIX2,
	TEXTURE_MATRIX3,
	TEXTURE_MATRIX4,
	TEXTURE_MATRIX5,
	TEXTURE_MATRIX6,
	TEXTURE_MATRIX7,
};

namespace Matrix
{
	float* GetMatrix();
	float* GetMatrix(MatrixType mode);

	int MatrixMode(MatrixType mode);

	void LoadIdentity();
	void Multiply(float mat[16]);
	void Translate(float x, float y, float z);
	void Rotate(float angle, float x, float y, float z);
	void Scale(float x, float y, float z);

	int Push();
	int Pop();

	// projection stuff
	void Perspective(float fov, float aspect, double near, double far);

	// debug
	void Print();

	// utils
	bool Invert4(float inverse[4][4], float mat[4][4]);
	bool Invert(); // invert the current matrix

	// vec3 overloads
	void Translate(vec3 pos);
	void Rotate(vec3 rot);
	void Scale(vec3 size);
}

#endif
