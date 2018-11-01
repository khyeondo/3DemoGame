#pragma once

#include "Vec3.h"

//참고:http://eirenehue.egloos.com/984622
struct Camera
{
public:
	//카메라 위치
	Vec3 pos = { 0.f,0.f,-10.f };

	//카메라 방향
	Vec3 lookAt = { 0,0,0 };
	 
	//카메라 최소 시야 
	float near;

	//카메라 최대 시야
	float far;

	//카메라 시야각
	float fov;
};