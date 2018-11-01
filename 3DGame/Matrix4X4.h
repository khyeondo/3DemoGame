#pragma once

#include <math.h>

class Vec3;

struct Matrix4X4
{
public:
	//[ї­][За]
	float m[4][4] = { 0 };

public:
	static void MakeProjectionMatrix(Matrix4X4& mat, int fov, int near, int far, int screenW, int screenH);
	static void MakeLookAtMatrix(Matrix4X4& mat, Vec3& pos, Vec3& lookAt, Vec3 up);
	static void MakeViewPortMatrix(Matrix4X4& mat, int screenW, int screenH);
	static void MakeScalingMatrix(Matrix4X4& mat, float sizeX, float sizeY, float sizeZ);

	static void MakeRotationX(Matrix4X4& mat, float angle);
	static void MakeRotationY(Matrix4X4& mat, float angle);
	static void MakeRotationZ(Matrix4X4& mat, float angle);


	static void MatrixMultiplyMatrix(Matrix4X4 &in1, Matrix4X4 &in2, Matrix4X4 &out);
};


