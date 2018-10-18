#pragma once
#include <math.h>

//row followed by column
struct Matrix4X4
{	
public:
	float m[4][4] = { 0 };

public:
	static Matrix4X4 MakeRotationX(Matrix4X4& mat, float angle)
	{
		/*
		1		0		0		0
		0		cos		sin		0
		0	    -sin	cos		0
		0		0		0		1
		*/
		mat.m[0][0] = 1;
		mat.m[1][1] = cosf(angle);
		mat.m[1][2] = sinf(angle);
		mat.m[2][1] = -sinf(angle);
		mat.m[2][2] = cosf(angle);
		mat.m[3][3] = 1;
		return mat;
	}
	static Matrix4X4 MakeRotationY(Matrix4X4& mat, float angle)
	{
		/*
		cos		0		sin		0
		0		1		0		0
		-sin	0 		cos		0
		0		0		0		1
		*/
		mat.m[0][0] = cosf(angle);
		mat.m[0][2] = sinf(angle);
		mat.m[2][0] = -sinf(angle);
		mat.m[1][1] = 1.0f;
		mat.m[2][2] = cosf(angle);
		mat.m[3][3] = 1.0f;
		return mat;
	}
	static Matrix4X4 MakeRotationZ(Matrix4X4& mat, float angle)
	{
		/*
		cos		sin		0		0
		-sin	cos		0		0
		0	    0		1		0
		0		0		0		1
		*/
		mat.m[0][0] = cosf(angle);
		mat.m[0][1] = sinf(angle);
		mat.m[1][0] = -sinf(angle);
		mat.m[1][1] = cos(angle);
		mat.m[2][2] = 1;
		mat.m[3][3] = 1;
		return mat;
	}

	//static Matrix4X4 Matrix_MultiplyMatrix(Matrix4X4 &m1, Matrix4X4 &m2)
	//{
	//	Matrix4X4 matrix;
	//	for (int c = 0; c < 4; c++)
	//		for (int r = 0; r < 4; r++)
	//			matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
	//	return matrix;
	//}
};
