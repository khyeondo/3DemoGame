#pragma once

#include "Vec3.h"

struct Camera
{
	//ī�޶� ��ġ
	Vec3 pos = { 0.f,0.f,-10.f };

	//ī�޶� ����
	Vec3 angle = { 0.f,0.f,0.f };

	//ī�޶� �ּ� �þ� 
	float near;

	//ī�޶� �ּ� �þ�
	float far;

	//ī�޶� �þ߰� (Field of View)
	float fov;
};