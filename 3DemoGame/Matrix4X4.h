#pragma once
#include <math.h>

//row followed by column
struct Matrix4X4
{	
	float m[4][4] = { 0 };

	static Matrix4X4 GetRotateX(float angle) {
		/*
		1		0		0		0
		0		cos		sin		0
		0	    -sin	cos		0
		0		0		0		1
		*/
		Matrix4X4 mat;
		mat.m[0][0] = 1;
		mat.m[1][1] = cosf(angle);
		mat.m[1][2] = sinf(angle);
		mat.m[2][1] = -sinf(angle);
		mat.m[2][2] = cosf(angle);
		mat.m[3][3] = 1;
		return mat;
	}
	static Matrix4X4 GetRotateY(float angle) {
		/*
		cos		0		-sin	0
		0		1		0		0
		sin	    0		cos		0
		0		0		0		1
		*/
		Matrix4X4 mat;
		mat.m[0][0] = cosf(angle);
		mat.m[0][2] = -sinf(angle);
		mat.m[1][1] = 1;
		mat.m[2][0] = sinf(angle);
		mat.m[2][2] = cosf(angle);
		mat.m[3][3] = 1;
		return mat;
	}
	static Matrix4X4 GetRotateZ(float angle){
		/*
		cos		sin		0		0
		-sin	cos		0		0
		0	    0		1		0
		0		0		0		1
		*/
		Matrix4X4 mat;
		mat.m[0][0] = cosf(angle);
		mat.m[0][1] = sinf(angle);
		mat.m[1][0] = -sinf(angle);
		mat.m[1][1] = cos(angle);
		mat.m[2][2] = 1;
		mat.m[3][3] = 1;
		return mat;
	}
};
