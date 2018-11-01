#pragma once

#include "Vec3.h"


class Camera
{
public:
	enum {
		X,Y,Z
	};

private:
	Vec3 m_pos;
	Vec3 m_lookAt;

	//카메라 로컬 축
	Vec3 m_axis[3];

	//카메라 최소 시야 
	float near;
	//카메라 최대 시야
	float far;
	//카메라 시야각
	float fov;

private:
	Camera();

public:
	static Camera* Create(Vec3 pos, Vec3 lookAt);

};