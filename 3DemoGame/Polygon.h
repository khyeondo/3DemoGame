#pragma once
#include "Vec3.h"
#include "Structs.h"

struct Polygon
{
public:
	Vec3 vertex[3];
	Vec2 uv[3];
	Vec3 normalVec;
	float brightness;
	float depth;

public:
	void Translate(Vec3& vec){
		for (int i = 0; i < 3; i++)
		{
			vertex[i].x += vec.x;
			vertex[i].y += vec.y;
			vertex[i].z += vec.z;
		}
	}
};