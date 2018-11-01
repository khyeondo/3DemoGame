#pragma once

#include <math.h>
#include "Vec3.h"

struct Matrix4X4
{
public:
	//[열][행]
	float m[4][4] = { 0 };

public:
	static void MakeProjectionMatrix(Matrix4X4& mat, int fov, int near, int far, int screenW, int screenH)
	{
		float screenRatio = (float)screenH / (float)screenW;
		float fovTan = 1.0f / tanf(fov*0.5f / 180.0f* 3.141592f);

		mat.m[0][0] = screenRatio * fovTan;
		mat.m[1][1] = fovTan;
		mat.m[2][2] = far / (far - near);
		mat.m[3][2] = (-far * near) / (far - near);
		mat.m[2][3] = 1.f;
	}
	//참고:https://docs.microsoft.com/en-us/windows/desktop/direct3d9/d3dxmatrixlookatlh
	static void MakeLookAtMatrix(Matrix4X4& mat, Vec3& pos, Vec3& lookAt, Vec3& up) {
		Vec3 z = (lookAt - pos).Normalize();
		Vec3 x = Vec3::CrossProduct(up, z).Normalize();
		Vec3 y = Vec3::CrossProduct(z, x);

		mat.m[0][0] = x.x; mat.m[1][0] = y.x; mat.m[2][0] = z.x; mat.m[3][0] = 0.f;
		mat.m[0][1] = x.y; mat.m[1][1] = y.y; mat.m[2][1] = z.y; mat.m[3][1] = 0.f;
		mat.m[0][2] = x.z; mat.m[1][2] = y.z; mat.m[2][2] = z.z; mat.m[3][2] = 0.f;
		mat.m[0][3] = -Vec3::DotProduct(x, pos); mat.m[1][3] = -Vec3::DotProduct(y, pos);
		mat.m[2][3] = -Vec3::DotProduct(z, pos); mat.m[3][3] = 1.f;
	}
	static void MakeViewPortMatrix(Matrix4X4& mat, int screenW, int screenH)
	{
		mat.m[0][0] = screenW / 2.f;
		mat.m[1][1] = -screenH / 2.f;
		mat.m[3][0] = screenW / 2.f;
		mat.m[3][1] = screenH / 2.f;
	}
	static void MakeScalingMatrix(Matrix4X4& mat, float sizeX, float sizeY, float sizeZ) {
		mat.m[0][0] = sizeX;
		mat.m[1][1] = sizeY;
		mat.m[2][2] = sizeZ;
		mat.m[3][3] = 1.f;
	}

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


