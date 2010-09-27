#include <GL/glew.h>
#include "Video.h"
#include <math.h>
#include "MathUtils.h"

#ifndef SWAP
#define SWAP(type, a, b)	{ type tmp=(a); (a)=(b); (b)=tmp; }
#endif

using namespace Video;

// TODO: store matrix stuff for convenience.
void Matrix::Push()
{
	glPushMatrix();
}

void Matrix::Pop()
{
	glPopMatrix();
}

void Matrix::Translate(float x, float y, float z)
{
	float translate[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, y, z, 1 };
	glMultMatrixf(translate);
}

void Matrix::Scale(float x, float y, float z)
{
	float scale[16] = { x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1 };
	glMultMatrixf(scale);
}

void Matrix::Rotate(float angle, float x, float y, float z)
{
	float c = cosf(radf(angle));
	float s = sinf(radf(angle));
	float oc = 1.0f - c;
	float xs, ys, zs;
	xs = x*s; ys = y*s; zs = z*s;
	float rotate[16] = {
		x*x*oc + c,	y*x*oc + zs,	(x*z*oc) - ys,	0.f,
		x*y*oc - zs,	y*y*oc + c,	(y*z*oc) + xs,	0.f,
		x*z*oc + ys,	y*z*oc - xs,	z*z*oc + c, 0.f,
		0, 0, 0, 1
	};
	glMultMatrixf(rotate);
}

bool Matrix::Invert4(float inverse[4][4], float mat[4][4])
{
	double temp;
	float tempmat[4][4], max;
	int maxj;

	// Set inverse to identity
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			inverse[i][j] = 0;
	for (int i=0; i<4; i++)
		inverse[i][i] = 1;

	// Copy original matrix so we don't mess it up
	for (int i = 0; i < 4; i++)
		for (int j = 0; j <4; j++)
			tempmat[i][j] = mat[i][j];

	for (int i = 0; i < 4; i++)
	{
		// Look for row with max pivot
		max = fabs(tempmat[i][i]);
		maxj = i;
		for (int j = i + 1; j < 4; j++)
		{
			if (fabs(tempmat[j][i]) > max)
			{
				max = fabs(tempmat[j][i]);
				maxj = j;
			}
		}
		// Swap rows if necessary
		if (maxj != i)
		{
			for (int k = 0; k < 4; k++)
			{
				SWAP(float, tempmat[i][k], tempmat[maxj][k]);
				SWAP(float, inverse[i][k], inverse[maxj][k]);
			}
		}

		temp = tempmat[i][i];
		if (temp == 0)
			return false; // No non-zero pivot
		for (int k = 0; k < 4; k++)
		{
			tempmat[i][k] = tempmat[i][k]/temp;
			inverse[i][k] = inverse[i][k]/temp;
		}
		for (int j = 0; j < 4; j++)
		{
			if (j != i)
			{
				temp = tempmat[j][i];
				for (int k = 0; k < 4; k++)
				{
					tempmat[j][k] -= tempmat[i][k]*temp;
					inverse[j][k] -= inverse[i][k]*temp;
				}
			}
		}
	}
	return true;
}

void Matrix::f16_m4(float m4[4][4], float f16[16])
{
	m4[0][0] = f16[0];
	m4[0][1] = f16[1];
	m4[0][2] = f16[2];
	m4[0][3] = f16[3];

	m4[1][0] = f16[5];
	m4[1][1] = f16[6];
	m4[1][2] = f16[7];
	m4[1][3] = f16[8];

	m4[2][0] = f16[9];
	m4[2][1] = f16[10];
	m4[2][2] = f16[11];
	m4[2][3] = f16[12];

	m4[3][0] = f16[13];
	m4[3][1] = f16[14];
	m4[3][2] = f16[15];
	m4[3][3] = f16[16];
}

void Matrix::m4_f16(float f16[16], float m4[4][4])
{
	f16[0] = m4[0][0];
	f16[1] = m4[0][1];
	f16[2] = m4[0][2];
	f16[3] = m4[0][3];

	f16[4] = m4[1][0];
	f16[5] = m4[1][1];
	f16[6] = m4[1][2];
	f16[7] = m4[1][3];

	f16[8] = m4[2][0];
	f16[9] = m4[2][1];
	f16[10] = m4[2][2];
	f16[11] = m4[2][3];

	f16[12] = m4[3][0];
	f16[13] = m4[3][1];
	f16[14] = m4[3][2];
	f16[15] = m4[3][3];
}
