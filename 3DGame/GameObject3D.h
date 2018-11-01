#pragma once

#include "GameObject.h"
#include "Vec3.h"
#include "Color.h"

class SDL_Surface;
class Mesh;

class GameObject3D : public GameObject
{
private:
	SDL_Surface* m_pSurface;
	Mesh* m_pMesh;
	Color m_color;
	Vec3 m_pos = { 0.f,0.f,0.f };
	Vec3 m_scale = { 1.f,1.f,1.f };
	Vec3 m_angle = { 0.f,0.f,0.f };

public:
	GameObject3D(SDL_Surface* surface, Mesh* mesh) : GameObject(),m_pSurface(surface), m_pMesh(mesh) {}
	virtual ~GameObject3D() {}

	virtual void Init();
	virtual void Update();
	virtual void Render();

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
		return m_scale;
	}

	Vec3& RefPos() {
		return m_pos;
	}
	Vec3& RefAngle() {
		return m_angle;
	}
	Vec3& RefScale() {
		return m_scale;
	}
};