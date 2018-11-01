
#include "Renderer3D.h"
#include "Camera.h"
#include "GameObject3D.h"
#include "Mesh.h"
#include <SDL.h>

Renderer3D* Renderer3D::inst = 0;

Renderer3D::~Renderer3D()
{
	SDL_FreeSurface(m_pScreenBuffer);
	delete[] m_pDepthBuffer;
}

bool Renderer3D::Init(SDL_Renderer * pRenderer, Camera * pCamera, Vec3 light, int screenW, int screenH)
{
	m_pRenderer = pRenderer;

	m_screenW = screenW;
	m_screenH = screenH;

	m_light = light;
	light.Normalize();

	m_pCamera = pCamera;
	m_pCamera->near = 0.1f;
	m_pCamera->far = 1000.0f;
	m_pCamera->fov = 90.0f;

	Matrix4X4::MakeProjectionMatrix(m_matProj, m_pCamera->fov, m_pCamera->near, 
		m_pCamera->far, m_screenW, m_screenH);
	Matrix4X4::MakeViewPortMatrix(m_matViewport, m_screenW, m_screenH);

	if ((m_pScreenBuffer = SDL_CreateRGBSurface(0, screenW, screenH, 32, 0, 0, 0, 0)) == 0)
		return false;
	
	m_pDepthBuffer = new float[screenW*screenH];
	for (int i = 0; i < screenH*screenW; i++)
		m_pDepthBuffer[i] = 0;

	light.Normalize();

	return true;
}

void Renderer3D::Rendering(GameObject3D * pGameObject3D)
{
	vector<Polygon> polys;
	vector<reference_wrapper<Polygon>> culledPolys;

	WorldSpace(pGameObject3D, polys);
	ViewSpace(polys);
}

void Renderer3D::Render3DPresent()
{
	SDL_Texture* pTexture;
	pTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_pScreenBuffer);

	SDL_Rect rect;
	rect.x = rect.y = 0;
	rect.w = m_screenW;
	rect.h = m_screenH;
	SDL_RenderCopy(m_pRenderer, pTexture, &rect, &rect);
	SDL_DestroyTexture(pTexture);
}

void Renderer3D::WorldSpace(GameObject3D * pGameObject, vector<Polygon>& polys)
{
	Matrix4X4 scaling;
	Matrix4X4 rotateX, rotateY, rotateZ;

	Matrix4X4::MakeScalingMatrix(scaling,
		pGameObject->GetScale().x, pGameObject->GetScale().y, pGameObject->GetScale().z);
	Matrix4X4::MakeRotationX(rotateX, pGameObject->RefAngle().x);
	Matrix4X4::MakeRotationY(rotateX, pGameObject->RefAngle().y);
	Matrix4X4::MakeRotationZ(rotateX, pGameObject->RefAngle().z);

	//크기 -> 회전(z->x->y) -> 이동
	for (Polygon poly : pGameObject->GetMesh()->polys)
	{
		for (int i = 0; i < 3; i++)
		{
			poly.vertex[i] *= scaling;
			poly.vertex[i] *= rotateZ;
			poly.vertex[i] *= rotateX;
			poly.vertex[i] *= rotateY;

			poly.vertex[i] -= pGameObject->RefPos();
		}
		polys.push_back(poly);
	}
}

void Renderer3D::ViewSpace(vector<Polygon>& polys)
{
	for (Polygon& poly : polys)
	{
		poly.vertex[0] *= m_matLootAt;
		poly.vertex[1] *= m_matLootAt;
		poly.vertex[2] *= m_matLootAt;
	}
}

void Renderer3D::BackfaceCulling(vector<Polygon>& polys, vector<Polygon>& culledPolys)
{

}



