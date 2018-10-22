#pragma once

#include "Vec3.h"

struct Camera
{
public:
	//ī�޶� ��ġ
	Vec3 pos = { 0.f,0.f,-10.f };

	//ī�޶� ����
	Vec3 lookDir = { 0,0,0 };

	//ī�޶� ����
	Vec3 angle = { 0.f,0.f,0.f };

	//ī�޶� �ּ� �þ� 
	float near;

	//ī�޶� �ִ� �þ�
	float far;

	//ī�޶� �þ߰�
	float fov;

public:
	Vec3 GetRight() {
		return Vec3(lookDir.z, lookDir.y, -lookDir.x);
	}

};