#pragma once

#include "GameObject.h"
#include "Vec3.h"
#include "Color.h"

class SDL_Surface;
class Mesh;

class GameObject3D : GameObject
{
private:
	SDL_Surface* m_pSurface;
	Mesh* m_pMesh;
	Color m_Color;
	Vec3 m_Pos = { 0.f,0.f,0.f };
	Vec3 m_Scale = { 1.f,1.f,1.f };
	Vec3 m_Angle = { 0.f,0.f,0.f };

public:
	GameObject3D(SDL_Surface* surface, Mesh* mesh) : m_pSurface(surface), m_pMesh(mesh) {}

	virtual void Init();
	virtual void Update();
	void Render();

	void SetMesh(Mesh* mesh) {
		m_pMesh = mesh;
	}
	void SetSurface(SDL_Surface* surface) {
		m_pSurface = surface;
	}

	Mesh* GetMesh() {
		return m_pMesh;
	}
	SDL_Surface* GetSurface() {
		return m_pSurface;
	}
	Vec3 GetScale() {
		return m_Scale;
	}

	Vec3& RefPos() {
		return m_Pos;
	}
	Vec3& RefAngle() {
		return m_Angle;
	}
};