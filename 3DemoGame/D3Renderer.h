#pragma once

#include "defulte.h"
#include "Camera.h"

class D3Renderer
{
#pragma region Single
private:
	static D3Renderer * m_pinst;
public:
	static D3Renderer * GetInst() {
		if (m_pinst == 0)
			m_pinst = new D3Renderer();
		return m_pinst;
	}
	static void DeleteSingle() {
		if (m_pinst != 0)
			delete m_pinst;
		m_pinst = 0;
	}
#pragma endregion
public:
	Camera camera;

private:

	int screenWidth, screenHeight;

	//�������� ���
	Matrix4X4 matProj;

	vector<Polygon> vecPoly;

	struct _Polygon
	{
		Polygon poly;
		Vec3 normalVec;
		float depth;
	};

private:
	bool CullOff(Vec3 a, Vec3 b, Vec3 c); //�ĸ� ó��
	void DrawPolygon(SDL_Renderer* pRender, Vec3* p, Color color, float b);
	void WorldToCamera(Mesh& mesh);

public:
	Matrix4X4 GetMatProj() {
		return matProj;
	}

	void SetCamera(Camera camera) {
		this->camera = camera;
	}
	

	bool Init(int screenH, int screenW);
	void RenderPresent(SDL_Renderer* pRenderer);
	void Draw(D3Object& object);
};