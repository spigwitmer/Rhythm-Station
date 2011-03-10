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

static const float identity_matrix[16] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

Matrix::Matrix()
{
	this->Identity();
}

void Matrix::Load(float *m)
{
	memcpy(&matrix[0][0], m, sizeof(identity_matrix));
}

void Matrix::Zero()
{
	memset(&matrix[0][0], 0, sizeof(identity_matrix));
}

void Matrix::Identity()
{
	memcpy(&matrix[0][0], identity_matrix, sizeof(identity_matrix));
}

// implemented from GL2.1 documentation
void Matrix::Translate(float x, float y, float z)
{
	glm::mat4 mat;
	mat[0] = vec4(1, 0, 0, 0);
	mat[1] = vec4(0, 1, 0, 0);
	mat[2] = vec4(0, 0, 1, 0);
	mat[3] = vec4(x, y, z, 1);

	this->matrix *= mat;
}

void Matrix::Scale(float x, float y, float z)
{
	glm::mat4 mat;
	mat[0] = vec4(x, 0, 0, 0);
	mat[1] = vec4(0, y, 0, 0);
	mat[2] = vec4(0, 0, z, 0);
	mat[3] = vec4(0, 0, 0, 1);

	this->matrix *= mat;
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
	
	glm::mat4 mat;
	mat[0] = vec4(x * x * oc + c, y * x * oc + zs, (x * z * oc) - ys, 0);
	mat[1] = vec4(x * y * oc - zs, y * y * oc + c, (y * z * oc) + xs, 0);
	mat[2] = vec4(x * z * oc + ys, y * z * oc - xs, z * z * oc + c, 0);
	mat[3] = vec4(0, 0, 0, 1);
	
	this->matrix *= mat;
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
	
	glm::mat4 mat;
	mat[0] = vec4(2.f / (right - left), 0, 0, 0);
	mat[1] = vec4(0, 2.f / (top - bottom), 0, 0);
	mat[2] = vec4(0, 0, -2.f / (far - near), 0);
	mat[3] = vec4(tx, ty, tz, 1);

	this->matrix *= mat;
}

// based on Mesa - there is a way to do this with less trig, but I don't remember.
void Matrix::Perspective(float fov, float aspect, double near, double far)
{
	float radians = radf(fov * 0.5f);
	float sine = sinf(radians), delta = far - near, ctan = 0.0f;
	delta = far - near;
	
	if (delta == 0 || sine == 0 || aspect == 0)
		return;
		
	ctan = cos(radians) / sine;
	glm::mat4 mat;
	
	mat[0].x = ctan / aspect;
	mat[1].y = ctan;
	mat[2].z = -(far + near) / delta;
	mat[2].w = -1;
	mat[3].z = -2 * near * far / delta;
	
	this->matrix *= mat;
}

void Matrix::LookAt(vec3 eye, vec3 center, vec3 up)
{
	vec3 forward, side;
	glm::mat4 mat;
	forward = center;
	
	// side = forward x up
	side = glm::cross(forward, up);
	side = glm::normalize(side);
	
	// up = side x forward
	up = glm::cross(side, forward);
	forward = -forward;
	
	this->Identity();

	mat[0] = vec4(side.x, side.y, side.z, 0);
	mat[1] = vec4(up.x, up.y, up.z, 0);
	mat[2] = vec4(forward.x, forward.y, forward.z, 0);
	mat[3].w = 1;
	
	mat = glm::transpose(mat);
	this->matrix *= mat;	
	this->Translate(-eye);
}
