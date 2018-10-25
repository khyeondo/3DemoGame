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
	Vec3 light = { 1.0f, -1.0f, 0.0f };

	//test
	SDL_Surface* surface;
	//

private:
	int screenWidth, screenHeight;
	
	float* pDepthBuffer = 0;

	//원근투영 행렬
	Matrix4X4 matProj;
	//카메라 좌표계 행렬
	Matrix4X4 matLootAt;
	//뷰포트 행렬
	Matrix4X4 matViewport;

	vector<Polygon> vecPoly;
	vector<reference_wrapper<Polygon>> vecInCameraPoly;
	vector<reference_wrapper<Polygon>> vecCulledPoly;

private:
	D3Renderer() {}
	~D3Renderer() {}

	void RenderingPipeline(SDL_Renderer* pRenderer);

	void LocalToWorld(Mesh& mesh,Vec3 objPos,Vec3 objAngle);
	void WorldToCamera();
	void CameraToViewer();
	void Culling();
	void Shading();
	void DepthSort();
	void ViewerToViewport();
	void OutPut(SDL_Renderer* pRenderer);

	void DrawPolygon(SDL_Renderer* pRenderer, Vec3* p, Color color, float b);
	bool CullOff(Polygon& poly); //후면 처리

	Uint32 GetPixel(SDL_Surface *surface, int x, int y);
	void TexturedTriangle(int x1, int y1, float u1, float v1, float w1,
		int x2, int y2, float u2, float v2, float w2,
		int x3, int y3, float u3, float v3, float w3,
		SDL_Renderer* pRenderer, SDL_Surface *surface);

public:
	bool Init(int screenH, int screenW);
	void Draw(D3Object& object);
	void RenderPresent(SDL_Renderer* pRenderer);
	
	void SetCamera(Camera camera) {
		this->camera = camera;
	}
	Matrix4X4 GetMatProj() {
		return matProj;
	}
};