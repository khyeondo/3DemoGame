#pragma once

#include "Vec3.h"

//����:http://eirenehue.egloos.com/984622
struct Camera
{
public:
	Camera(Vec3 pos, Vec3 lookAt, float near,float far,float fov):
		pos(pos),lookAt(lookAt),near(near),far(far),fov(fov)
	{
	}

	//ī�޶� ��ġ
	Vec3 pos = { 0.f,0.f,0.f };

	//ī�޶� ����
	Vec3 lookAt = { 0,0,0 };
	 
	//ī�޶� �ּ� �þ� 
	float near;

	//ī�޶� �ִ� �þ�
	float far;

	//ī�޶� �þ߰�
	float fov;
};