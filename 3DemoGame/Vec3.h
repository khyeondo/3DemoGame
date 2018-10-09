#pragma once
#include "Matrix4X4.h"

struct Vec3
{
public:
	float x, y, z = 0;
	float w = 1;

public:
	Vec3() {}
	Vec3(float x, float y, float z,float w = 1)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
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

		return temp;
	}
	Vec3 operator * (Matrix4X4 mat) {
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
	void operator *= (Matrix4X4 mat) {
		Vec3 temp;
		temp.x = x * mat.m[0][0] + y * mat.m[1][0] + z * mat.m[2][0] + w * mat.m[3][0];
		temp.y = x * mat.m[0][1] + y * mat.m[1][1] + z * mat.m[2][1] + w * mat.m[3][1];
		temp.z = x * mat.m[0][2] + y * mat.m[1][2] + z * mat.m[2][2] + w * mat.m[3][2];
		temp.w = x * mat.m[0][3] + y * mat.m[1][3] + z * mat.m[2][3] + w * mat.m[3][3];
		x = temp.x;
		y = temp.y;
		z = temp.z;
		w = temp.w;
	}
	void operator /= (float f)
	{
		x = x / f;
		y = y / f;
		z = z / f;
	}
	
	//크기
	float Length() {
		return sqrt(powf(x, 2.f) + powf(y, 2.f) + powf(z, 2.f));
	}

	//정규화
	Vec3 Normalize(){
		float f = this->Length();
		x /= f; y /= f; z /= f;
		return *this;
	}

	//외적
	static Vec3 CrossProduct(Vec3 v1, Vec3 v2){
		Vec3 temp;
		temp.x = v1.y * v2.z - v1.z * v2.y;
		temp.y = v1.z * v2.x - v1.x * v2.z;
		temp.z = v1.x * v2.y - v1.y * v2.x;
		return temp;
	}

	//내적
	static float DotProduct(Vec3 v1, Vec3 v2) {
		return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z;
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

