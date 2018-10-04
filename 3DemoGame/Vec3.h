#pragma once
#include "Matrix4X4.h"

struct Vec3
{
public:
	float x, y, z, w = 1;

public:
	Vec3() {}
	Vec3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vec3 operator + (Vec3 vec)
	{
		Vec3 temp;
		temp.x = x + vec.x;
		temp.y = y + vec.y;
		temp.z = z + vec.z;
		return temp;
	}
	Vec3 operator - (Vec3 vec) {
		Vec3 temp;
		temp.x = x - vec.x;
		temp.y = y - vec.y;
		temp.z = z - vec.z;
		return temp;
	}
	Vec3 operator * (float f)
	{
		Vec3 temp;
		temp.x = x * f;
		temp.y = y * f;
		temp.z = z * f;
	}
	Vec3 operator * (Matrix4X4 &mat) {
		Vec3 temp;
		temp.x = x * mat.m[0][0] + y * mat.m[1][0] + z * mat.m[2][0] + w * mat.m[3][0];
		temp.y = x * mat.m[0][1] + y * mat.m[1][1] + z * mat.m[2][1] + w * mat.m[3][1];
		temp.z = x * mat.m[0][2] + y * mat.m[1][2] + z * mat.m[2][2] + w * mat.m[3][2];
		temp.w = x * mat.m[0][3] + y * mat.m[1][3] + z * mat.m[2][3] + w * mat.m[3][3];
		return temp;
	}
	Vec3 operator / (float f)
	{
		Vec3 temp;
		temp.x = x / f;
		temp.y = y / f;
		temp.z = z / f;
	}
	void operator += (Vec3 vec) {
		x = x + vec.x;
		y = y + vec.y;
		z = z + vec.z;
	}
	void operator -= (Vec3 vec) {
		x = x - vec.x;
		y = y - vec.y;
		z = z - vec.z;
	}
	void operator *= (float f)
	{
		x = x * f;
		y = y * f;
		z = z * f;
	}
	void operator /= (float f)
	{
		x = x / f;
		y = y / f;
		z = z / f;
	}
	
	float Length() {
		return sqrt(powf(x, 2.f) + powf(y, 2.f) + powf(z, 2.f));
	}
	//정규화
	Vec3 Normalize()
	{
		float f = this->Length();
		x /= f; y /= f; z /= f;
		return *this;
	}
	//외적
	static Vec3 Cross(Vec3 a, Vec3 b)
	{
		Vec3 temp;
		temp.x = a.y * b.z - a.z * b.y;
		temp.y = a.z * b.x - a.x * b.z;
		temp.z = a.x * b.y - a.y * b.x;
		return temp;
	}

	//축회전
	void RotateX(float angle)
	{
		float tempY = y;
		float tempZ = z;
		y = tempY * cosf(angle) + tempZ * sinf(angle);
		z = tempZ * cosf(angle) - tempY * sinf(angle);
	}
	void RotateY(float angle)
	{
		float tempX = x;
		float tempZ = z;
		x = tempX * cosf(angle) + tempZ * sinf(angle);
		z = tempZ * cosf(angle) - tempX * sinf(angle);
	}
	void RotateZ(float angle)
	{
		float tempX = x;
		float tempY = y;
		x = tempX * cosf(angle) + tempY * sinf(angle);
		y = tempY * cosf(angle) - tempX * sinf(angle);
	}

};

