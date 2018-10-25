#pragma once
#include <math.h>

//row followed by column
struct Matrix4X4
{	
public:
	float m[4][4] = { 0 };

public:
	static void MakeRotationX(Matrix4X4& mat, float angle)
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
	}
	static void MakeRotationY(Matrix4X4& mat, float angle)
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
	}
	static void MakeRotationZ(Matrix4X4& mat, float angle)
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
	}
	


	static void Matrix_MultiplyMatrix(Matrix4X4 &in1, Matrix4X4 &in2, Matrix4X4 &out)
	{
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				out.m[r][c] = in2.m[r][0] * in1.m[0][c] + in2.m[r][1] * in1.m[1][c] + in2.m[r][2] * in1.m[2][c] + in2.m[r][3] * in1.m[3][c];
	}
};
