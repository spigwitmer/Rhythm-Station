#include <GL/glew.h>
#include "Matrix.h"
#include <math.h>
#include "Log.h"
#include "MathUtils.h"

// stack size constants
const int stack_size = 64;
static const size_t matrix_size = sizeof(float)*16;
static const int matrix_count = sizeof(MatrixType);

// current matrix info
MatrixType matrix_mode = PROJECTION_MATRIX;
int stack_depth[matrix_count] = { 0 };

// the stack itself
float stack[matrix_count][stack_size][16] = { 0 };

static const float identity_matrix[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
float current_matrix[16] = { 0.0f };

// useful for debugging.
void Matrix::Print() {
	printf(
		"%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
		current_matrix[0], current_matrix[1], current_matrix[2], current_matrix[3],
		current_matrix[4], current_matrix[5], current_matrix[6], current_matrix[7],
		current_matrix[8], current_matrix[9], current_matrix[10], current_matrix[11],
		current_matrix[12], current_matrix[13], current_matrix[14], current_matrix[15]
	);
}

float* Matrix::GetMatrix() {
	return current_matrix;
}

float* Matrix::GetMatrix(MatrixType mode) {
	return stack[mode][stack_depth[mode]];
}

// save current matrix onto the stack
int Matrix::Push() {
	if (++stack_depth[matrix_mode] > stack_size) {
		stack_depth[matrix_mode] = stack_size;
		return STACK_OVERFLOW;
	}
	memcpy(stack[matrix_mode][stack_depth[matrix_mode]], current_matrix, matrix_size);

	return STACK_NORMAL;
}

// restore matrix from the stack
int Matrix::Pop() {
	if (--stack_depth[matrix_mode] < 0) {
		stack_depth[matrix_mode] = 0;
		return STACK_UNDERFLOW;
	}
	memcpy(current_matrix, stack[matrix_mode][stack_depth[matrix_mode]], matrix_size);

	return STACK_NORMAL;
}

int Matrix::MatrixMode(MatrixType mode) {
	if (mode > matrix_count) {
		return 1; // invalid
	}
	matrix_mode = mode;
	memcpy(current_matrix, stack[matrix_mode][stack_depth[matrix_mode]], matrix_size);

	return 0;
}

void Matrix::LoadIdentity() {
	memcpy(current_matrix, identity_matrix, matrix_size);
}

// adapted from mesa
void Matrix::Multiply(float mat[16]) {
	for (int i = 0; i < 4; i++) {
		const float
			c0 = current_matrix[i],
			c1 = current_matrix[4+i],
			c2 = current_matrix[8+i],
			c3 = current_matrix[12+i];

		int cur = 0;
		current_matrix[cur+i] = c0 * mat[cur] + c1 * mat[cur+1] + c2 * mat[cur+2] + c3 * mat[cur+3]; cur += 4;
		current_matrix[cur+i] = c0 * mat[cur] + c1 * mat[cur+1] + c2 * mat[cur+2] + c3 * mat[cur+3]; cur += 4;
		current_matrix[cur+i] = c0 * mat[cur] + c1 * mat[cur+1] + c2 * mat[cur+2] + c3 * mat[cur+3]; cur += 4;
		current_matrix[cur+i] = c0 * mat[cur] + c1 * mat[cur+1] + c2 * mat[cur+2] + c3 * mat[cur+3];
	}
}

void Matrix::Translate(float x, float y, float z) {
	float mat[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, y, z, 1 };
	Matrix::Multiply(mat);
}

void Matrix::Scale(float x, float y, float z) {
	float mat[16] = { x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1 };
	Matrix::Multiply(mat);
}

void Matrix::Rotate(float angle, float x, float y, float z) {
	float c = cosf(radf(angle));
	float s = sinf(radf(angle));
	float oc = 1.0f - c;
	float xs, ys, zs;
	xs = x*s; ys = y*s; zs = z*s;
	float mat[16] = {
		x*x*oc + c,	y*x*oc + zs,	(x*z*oc) - ys,	0.f,
		x*y*oc - zs,	y*y*oc + c,	(y*z*oc) + xs,	0.f,
		x*z*oc + ys,	y*z*oc - xs,	z*z*oc + c, 	0.f,
		0, 0, 0, 1
	};
	Matrix::Multiply(mat);
}

// vec3 overloads
void Matrix::Translate(vec3 pos) {
	Matrix::Translate(pos.x, pos.y, pos.z);
}

void Matrix::Rotate(vec3 rot) {
	Matrix::Rotate(rot.x, 1, 0, 0);
	Matrix::Rotate(rot.y, 0, 1, 0);
	Matrix::Rotate(rot.z, 0, 0, 1);
}

void Matrix::Scale(vec3 size) {
	Matrix::Scale(size.x, size.y, size.z);
}

// adapted from blender.
bool Matrix::Invert4(float inverse[4][4], float mat[4][4]) {
	float tempmat[4][4];

	memcpy(inverse, identity_matrix, matrix_size); // inverse = identity
	memcpy(tempmat, mat, matrix_size); // mat -> temp

	for (int i = 0; i < 4; i++) {
		// look for row with max pivot
		int max = fabs(tempmat[i][i]);
		int maxj = i;
		for (int j = i + 1; j < 4; j++) {
			if (fabs(tempmat[j][i]) > max) {
				max = fabs(tempmat[j][i]);
				maxj = j;
			}
		}
		// swap rows if necessary
		if (maxj != i) {
			for (int k = 0; k < 4; k++) {
				SWAP(float, tempmat[i][k], tempmat[maxj][k]);
				SWAP(float, inverse[i][k], inverse[maxj][k]);
			}
		}

		double temp = tempmat[i][i];
		if (temp == 0)
			return false; // I can't do that, dave.
		for (int k = 0; k < 4; k++) {
			tempmat[i][k] = tempmat[i][k]/temp;
			inverse[i][k] = inverse[i][k]/temp;
		}
		for (int j = 0; j < 4; j++) {
			if (j != i) {
				temp = tempmat[j][i];
				for (int k = 0; k < 4; k++) {
					tempmat[j][k] -= tempmat[i][k]*temp;
					inverse[j][k] -= inverse[i][k]*temp;
				}
			}
		}
	}
	return true;
}

bool Matrix::Invert() {
	float tmp[4][4];
	memcpy(tmp, current_matrix, matrix_size);
	bool success = Matrix::Invert4(tmp, tmp);
	if (success)
		memcpy(current_matrix, tmp, matrix_size);
	return success;
}

void Matrix::Perspective(float fov, float aspect, double near, double far) {
	float m[4][4] = { 0 };
	double sine, ctan, delta;
	double radians = radf(fov * 0.5f);
	delta = far - near;
	sine = sinf(radians);

	if (delta == 0 || sine == 0 || aspect == 0)
		return;
	ctan = cos(radians) / sine;

	m[0][0] = ctan / aspect;
	m[1][1] = ctan;
	m[2][2] = -(far + near) / delta;
	m[2][3] = -1;
	m[3][2] = -2 * near * far / delta;
	m[3][3] = 0;

	Matrix::Multiply(&m[0][0]);
}

/*
TODO:
(these are from mesa)
void GLAPIENTRY
gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
{
    glOrtho(left, right, bottom, top, -1, 1);
}

#define __glPi 3.14159265358979323846


void GLAPIENTRY
gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble m[4][4];
    double sine, cotangent, deltaZ;
    double radians = fovy / 2 * __glPi / 180;

    deltaZ = zFar - zNear;
    sine = sin(radians);
    if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) {
	return;
    }
    cotangent = COS(radians) / sine;

    __gluMakeIdentityd(&m[0][0]);
    m[0][0] = cotangent / aspect;
    m[1][1] = cotangent;
    m[2][2] = -(zFar + zNear) / deltaZ;
    m[2][3] = -1;
    m[3][2] = -2 * zNear * zFar / deltaZ;
    m[3][3] = 0;
    glMultMatrixd(&m[0][0]);
}

static void normalize(float v[3])
{
    float r;

    r = sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
    if (r == 0.0) return;

    v[0] /= r;
    v[1] /= r;
    v[2] /= r;
}

static void cross(float v1[3], float v2[3], float result[3])
{
    result[0] = v1[1]*v2[2] - v1[2]*v2[1];
    result[1] = v1[2]*v2[0] - v1[0]*v2[2];
    result[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

void GLAPIENTRY
gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,
	  GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy,
	  GLdouble upz)
{
    float forward[3], side[3], up[3];
    GLfloat m[4][4];

    forward[0] = centerx - eyex;
    forward[1] = centery - eyey;
    forward[2] = centerz - eyez;

    up[0] = upx;
    up[1] = upy;
    up[2] = upz;

    normalize(forward);

    // Side = forward x up
    cross(forward, up, side);
    normalize(side);

    // Recompute up as: up = side x forward
    cross(side, forward, up);

    __gluMakeIdentityf(&m[0][0]);
    m[0][0] = side[0];
    m[1][0] = side[1];
    m[2][0] = side[2];

    m[0][1] = up[0];
    m[1][1] = up[1];
    m[2][1] = up[2];

    m[0][2] = -forward[0];
    m[1][2] = -forward[1];
    m[2][2] = -forward[2];

    glMultMatrixf(&m[0][0]);
    glTranslated(-eyex, -eyey, -eyez);
}

*/