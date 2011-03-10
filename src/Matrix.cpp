#include <cstdio>
#include <cstring>
#include <math.h>
#include "Logger.h"
#include "Matrix.h"

float radf(float val)
{
	// cast as float so the conversion is explicit.
	val = float(val * 0.017453292f); // PI / 180.f
	return val;
}

static const float identity_matrix[4][4] = {
	{ 1, 0, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 0, 1, 0 },
	{ 0, 0, 0, 1 }
};

Matrix::Matrix()
{
	this->Identity();
}

// useful for debugging.
void Matrix::Print()
{
	for (int i = 0; i<4; i++)
	{
		int ind = i<<2;
		Log->Print("%f %f %f %f", matrix[ind], matrix[ind+1], matrix[ind+2], matrix[ind+3]);
	}
}

void Matrix::Load(float *m)
{
	memcpy(matrix, m, sizeof(matrix));
}

void Matrix::Zero()
{
	memset(matrix, 0, sizeof(matrix));
}

void Matrix::Identity()
{
	memcpy(matrix, identity_matrix, sizeof(matrix));
}

void Matrix::Multiply(const float *mat)
{
	for (int i = 0; i < 4; i++)
	{
		const float c0 = matrix[i], c1 = matrix[4+i],
		            c2 = matrix[8+i], c3 = matrix[12+i];
		            
		for (int cur = 0; cur < 16; cur+=4)
			matrix[cur+i] = c0 * mat[cur] + c1 * mat[cur+1] + c2 * mat[cur+2] + c3 * mat[cur+3];
	}
}

// implemented from GL2.1 documentation
void Matrix::Translate(float x, float y, float z)
{
	float mat[4][4] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ x, y, z, 1 }
	};
	this->Multiply(&mat[0][0]);
}

void Matrix::Scale(float x, float y, float z)
{
	float mat[4][4] = {
		{ x, 0, 0, 0 },
		{ 0, y, 0, 0 },
		{ 0, 0, z, 0 },
		{ 0, 0, 0, 1 }
	};
	this->Multiply(&mat[0][0]);
}

void Matrix::Rotate(float angle, float x, float y, float z)
{
	float c = cosf(radf(angle));
	float s = sinf(radf(angle));
	float oc = 1.0 - c;
	float xs, ys, zs;
	xs = x * s;
	ys = y * s;
	zs = z * s;
	float mat[4][4] = {
		{ x * x * oc + c, y * x * oc + zs, (x * z * oc) - ys, 0, },
		{ x * y * oc - zs, y * y * oc + c, (y * z * oc) + xs, 0, },
		{ x * z * oc + ys, y * z * oc - xs, z * z * oc + c, 0, },
		{ 0, 0, 0, 1 }
	};
	this->Multiply(&mat[0][0]);
}

// overloads
void Matrix::Translate(vec3 pos)
{
	this->Translate(pos.x, pos.y, pos.z);
}

void Matrix::Rotate(vec3 rot) {
	this->Rotate(rot.x, 1, 0, 0);
	this->Rotate(rot.y, 0, 1, 0);
	this->Rotate(rot.z, 0, 0, 1);
}

void Matrix::Scale(vec3 size)
{
	this->Scale(size.x, size.y, size.z);
}

void Matrix::Ortho(float left, float right, float bottom, float top, float near, float far)
{
	this->Identity();
	float tx, ty, tz;
	tx = -((right+left) / (right-left));
	ty = -((top+bottom) / (top-bottom));
	tz = -((far+near) / (far-near));
	float m[4][4] = {
		{ 2.f / (right - left), 0, 0, 0 },
		{ 0, 2.f / (top - bottom), 0, 0 },
		{ 0, 0, -2.f / (far - near), 0 },
		{ tx, ty, tz, 1 }
	};
	this->Multiply(&m[0][0]);
}

// based on Mesa - there is a way to do this with less trig, but I don't remember.
void Matrix::Perspective(float fov, float aspect, double near, double far)
{
	float m[4][4] = {{0}};
	float radians = radf(fov * 0.5f);
	float sine = sinf(radians), delta = far - near, ctan = 0.0f;
	delta = far - near;
	
	if (delta == 0 || sine == 0 || aspect == 0)
		return;
		
	ctan = cos(radians) / sine;
	m[0][0] = ctan / aspect;
	m[1][1] = ctan;
	m[2][2] = -(far + near) / delta;
	m[2][3] = -1;
	m[3][2] = -2 * near * far / delta;
	
	this->Multiply(&m[0][0]);
}

void Matrix::LookAt(vec3 eye, vec3 center, vec3 up)
{
	vec3 forward, side;
	float m[4][4];
	forward = center;
	
	// side = forward x up
	side = glm::cross(forward, up);
	side = glm::normalize(side);
	
	// up = side x forward
	up = glm::cross(side, forward);
	forward = -forward;
	
	this->Identity();
	m[0][0] = side.x;
	m[1][0] = side.y;
	m[2][0] = side.z;
	m[0][1] = up.x;
	m[1][1] = up.y;
	m[2][1] = up.z;
	m[0][2] = forward.x;
	m[1][2] = forward.y;
	m[2][2] = forward.z;
	
	this->Multiply(&m[0][0]);
	this->Translate(-eye);
}
