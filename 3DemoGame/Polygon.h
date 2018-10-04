#pragma once
#include "Vec3.h"

struct Polygon
{
public:
	Vec3 vertex[3];
	Vec3 normalVec;
	float depth;

public:
	void RotateX(float angle) {
		for (int i = 0; i < 3; i++)
		{
			vertex[i] = vertex[i] * Matrix4X4::GetRotateX(angle);
		}
	}
	void RotateY(float angle) {
		for (int i = 0; i < 3; i++)
		{
			vertex[i] = vertex[i] * Matrix4X4::GetRotateY(angle);
		}
	}
	void RotateZ(float angle) {
		for (int i = 0; i < 3; i++)
		{
			vertex[i] = vertex[i] * Matrix4X4::GetRotateZ(angle);
		}
	}

	void Translate(Vec3& vec){
		for (int i = 0; i < 3; i++)
		{
			vertex[i].x += vec.x;
			vertex[i].y += vec.y;
			vertex[i].z += vec.z;
		}
	}
};