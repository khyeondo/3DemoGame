#pragma once

#include "Vec3.h"

struct Camera
{
public:
	//카메라 위치
	Vec3 pos = { 0.f,0.f,-10.f };

	//카메라 방향
	Vec3 lookDir = { 0,0,0 };

	//카메라 각도
	Vec3 angle = { 0.f,0.f,0.f };

	//카메라 최소 시야 
	float near;

	//카메라 최대 시야
	float far;

	//카메라 시야각
	float fov;

public:
	Vec3 GetRight() {
		return Vec3(lookDir.z, lookDir.y, -lookDir.x);
	}

};