#pragma once

#include "Mesh.h"

class D3Object
{
private:
	Mesh * mesh;


public:	
	Vec3 pos = { 0.f,0.f,0.f };
	Vec3 angle = { 0.f,0.f,0.f };
	void SetMesh(Mesh* mesh) {
		this->mesh = mesh;
	}

	Mesh * GetMesh() {
		return mesh;
	}
	Vec3 GetPos() {
		return pos;
	}
	Vec3 GetAngle() {
		return angle;
	}
};