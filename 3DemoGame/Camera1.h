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

	//ī�޶� ���� ��
	Vec3 m_axis[3];

	//ī�޶� �ּ� �þ� 
	float near;
	//ī�޶� �ִ� �þ�
	float far;
	//ī�޶� �þ߰�
	float fov;

private:
	Camera();

public:
	static Camera* Create(Vec3 pos, Vec3 lookAt);

};