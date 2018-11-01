
#include "Renderer3D.h"
#include "Camera.h"
#include "GameObject3D.h"
#include "Mesh.h"

#include "SDL2_gfxPrimitives.h"

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
	BackfaceCulling(polys, culledPolys);
	ViewSpace(culledPolys);
	Projection(culledPolys);
	Viewport(culledPolys);
	DrawPolygons(culledPolys);
	//Texturing(pGameObject3D, culledPolys);

	polys.clear();
	culledPolys.clear();
}

void Renderer3D::Present()
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
	Matrix4X4::MakeRotationY(rotateY, pGameObject->RefAngle().y);
	Matrix4X4::MakeRotationZ(rotateZ, pGameObject->RefAngle().z);

	//크기 -> 회전(z->x->y) -> 이동
	for (Polygon poly : pGameObject->GetMesh()->polys)
	{
		for (int i = 0; i < 3; i++)
		{
			poly.vertex[i] *= scaling;
			poly.vertex[i] *= rotateZ;
			poly.vertex[i] *= rotateX;
			poly.vertex[i] *= rotateY;

			poly.vertex[i] += pGameObject->RefPos();
		}
		polys.push_back(poly);
	}
}

void Renderer3D::BackfaceCulling(vector<Polygon>& polys, vector<reference_wrapper<Polygon>>& culledPolys)
{
	for (Polygon& poly : polys)
	{
		Vec3 line1, line2;
		line1 = poly.vertex[1] - poly.vertex[0];
		line2 = poly.vertex[2] - poly.vertex[0];

		poly.normalVec = Vec3::CrossProduct(line1, line2).Normalize();

		Vec3 cameraRay = poly.vertex[0] - m_pCamera->pos;
		if (Vec3::DotProduct(poly.normalVec, cameraRay) < 0.0f)
		{
			culledPolys.push_back(poly);
		}
	}
}

void Renderer3D::ViewSpace(vector<reference_wrapper<Polygon>>& culledPolys)
{
	Matrix4X4::MakeLookAtMatrix(m_matLootAt, m_pCamera->pos, m_pCamera->lookAt, Vec3(0, 1, 0));
	for (Polygon& poly : culledPolys)
	{
		poly.vertex[0] *= m_matLootAt;
		poly.vertex[1] *= m_matLootAt;
		poly.vertex[2] *= m_matLootAt;
	}
}

void Renderer3D::Shading(vector<reference_wrapper<Polygon>>& culledPolys)
{
	for (auto& poly : culledPolys)
	{
		poly.get().brightness = poly.get().normalVec.x * m_light.x + poly.get().normalVec.y * m_light.y + poly.get().normalVec.z * m_light.z;
		poly.get().brightness *= -1;
		if (poly.get().brightness < 0)
			poly.get().brightness = 0;
		//poly.get().brightness += 1.f;
		//poly.get().brightness *= (1.f / 2.f);
	}
}

void Renderer3D::Projection(vector<reference_wrapper<Polygon>>& culledPolys)
{
	for (Polygon& poly : culledPolys)
	{
		poly.vertex[0] *= m_matProj;
		poly.vertex[1] *= m_matProj;
		poly.vertex[2] *= m_matProj;

		//poly.vertex[0].x /= poly.vertex[0].w;
		//poly.vertex[1].x /= poly.vertex[1].w;
		//poly.vertex[2].x /= poly.vertex[2].w;
		//								   
		//poly.vertex[0].y /= poly.vertex[0].w;
		//poly.vertex[1].y /= poly.vertex[1].w;
		//poly.vertex[2].y /= poly.vertex[2].w;
		poly.vertex[0].x /= poly.vertex[0].z;
		poly.vertex[0].y /= poly.vertex[0].z;
		poly.vertex[0].z /= poly.vertex[0].z;
										   
		poly.vertex[1].x /= poly.vertex[1].z;
		poly.vertex[1].y /= poly.vertex[1].z;
		poly.vertex[1].z /= poly.vertex[1].z;
										   
		poly.vertex[2].x /= poly.vertex[2].z;
		poly.vertex[2].y /= poly.vertex[2].z;
		poly.vertex[2].z /= poly.vertex[2].z;
	}
}

void Renderer3D::Viewport(vector<reference_wrapper<Polygon>>& culledPolys)
{
	for (Polygon& poly : culledPolys)
	{
		poly.vertex[0].w = 1.f;
		poly.vertex[1].w = 1.f;
		poly.vertex[2].w = 1.f;

		poly.vertex[0] *= m_matViewport;
		poly.vertex[1] *= m_matViewport;
		poly.vertex[2] *= m_matViewport;
	}
}

void Renderer3D::DrawPolygons(vector<reference_wrapper<Polygon>>& culledPolys)
{
	for (Polygon& poly : culledPolys)
	{
		DrawPolygon(poly.vertex, Color(255, 255, 255), poly.brightness);
	}
}

void Renderer3D::Texturing(GameObject3D * pGameObject, vector<reference_wrapper<Polygon>>& culledPolys)
{
	for (Polygon& poly : culledPolys)
	{
		if (poly.vertex[1].y < poly.vertex[0].y)
		{
			Swap(poly.vertex[0].y, poly.vertex[1].y);
			Swap(poly.vertex[0].x, poly.vertex[1].x);
			Swap(poly.uv[0].x, poly.uv[1].y);
			Swap(poly.uv[0].y, poly.uv[1].x);
			Swap(poly.vertex[0].z, poly.vertex[1].z);
		}

		if (poly.vertex[2].y < poly.vertex[0].y)
		{
			Swap(poly.vertex[0].y, poly.vertex[2].y);
			Swap(poly.vertex[0].x, poly.vertex[2].x);
			Swap(poly.uv[0].x, poly.uv[2].y);
			Swap(poly.uv[0].y, poly.uv[2].x);
			Swap(poly.vertex[0].z, poly.vertex[2].z);
		}

		if (poly.vertex[2].y < poly.vertex[1].y)
		{
			Swap(poly.vertex[1].y, poly.vertex[2].y);
			Swap(poly.vertex[1].x, poly.vertex[2].x);
			Swap(poly.uv[1].y, poly.uv[2].y);
			Swap(poly.uv[1].x, poly.uv[2].x);
			Swap(poly.vertex[1].z, poly.vertex[2].z);
		}

		int dy1 = poly.vertex[1].y - poly.vertex[0].y;
		int dx1 = poly.vertex[1].x - poly.vertex[0].x;
		float dv1 = poly.uv[1].x - poly.uv[0].y;
		float du1 = poly.uv[1].x - poly.uv[0].x;
		float dw1 = poly.vertex[1].z - poly.vertex[0].z;

		int dy2 = poly.vertex[2].y - poly.vertex[0].y;
		int dx2 = poly.vertex[2].x - poly.vertex[0].x;
		float dv2 = poly.uv[2].y - poly.uv[0].y;
		float du2 = poly.uv[2].x - poly.uv[0].x;
		float dw2 = poly.vertex[2].z - poly.vertex[0].z;

		float tex_u, tex_v, tex_w;

		float dax_step = 0, dbx_step = 0,
			du1_step = 0, dv1_step = 0,
			du2_step = 0, dv2_step = 0,
			dw1_step = 0, dw2_step = 0;

		if (dy1) dax_step = dx1 / (float)abs(dy1);
		if (dy2) dbx_step = dx2 / (float)abs(dy2);

		if (dy1) du1_step = du1 / (float)abs(dy1);
		if (dy1) dv1_step = dv1 / (float)abs(dy1);
		if (dy1) dw1_step = dw1 / (float)abs(dy1);

		if (dy2) du2_step = du2 / (float)abs(dy2);
		if (dy2) dv2_step = dv2 / (float)abs(dy2);
		if (dy2) dw2_step = dw2 / (float)abs(dy2);

		if (dy1)
		{
			for (int i = poly.vertex[0].y; i <= poly.vertex[1].y; i++)
			{
				int ax = poly.vertex[0].x + (float)(i - poly.vertex[0].y) * dax_step;
				int bx = poly.vertex[0].x + (float)(i - poly.vertex[0].y) * dbx_step;

				float tex_su = poly.uv[0].x + (float)(i - poly.vertex[0].y) * du1_step;
				float tex_sv = poly.uv[0].y + (float)(i - poly.vertex[0].y) * dv1_step;
				float tex_sw = poly.vertex[0].z + (float)(i - poly.vertex[0].y) * dw1_step;

				float tex_eu = poly.uv[0].x + (float)(i - poly.vertex[0].y) * du2_step;
				float tex_ev = poly.uv[0].y + (float)(i - poly.vertex[0].y) * dv2_step;
				float tex_ew = poly.vertex[0].z + (float)(i - poly.vertex[0].y) * dw2_step;

				if (ax > bx)
				{
					Swap(ax, bx);
					Swap(tex_su, tex_eu);
					Swap(tex_sv, tex_ev);
					Swap(tex_sw, tex_ew);
				}

				tex_u = tex_su;
				tex_v = tex_sv;
				tex_w = tex_sw;

				float tstep = 1.0f / ((float)(bx - ax));
				float t = 0.0f;

				for (int j = ax; j < bx; j++)
				{
					tex_u = (1.0f - t) * tex_su + t * tex_eu;
					tex_v = (1.0f - t) * tex_sv + t * tex_ev;
					tex_w = (1.0f - t) * tex_sw + t * tex_ew;
					//if (tex_w > pDepthBuffer[i*ScreenWidth() + j])
					//	{
					//Draw(j, i, tex->SampleGlyph(tex_u / tex_w, tex_v / tex_w), tex->SampleColour(tex_u / tex_w, tex_v / tex_w));
					//Uint32 color = getpixel(surface, (int)((tex_u)*surface->clip_rect.w), (int)((tex_v)*surface->clip_rect.h));
					//SDL_SetRenderDrawColor(pRenderer, (Uint8)((color & 0x00FF0000) >> 16),
					//	(Uint8)((color & 0x0000FF00) >> 8),
					//	(Uint8)((color & 0x000000FF)), (Uint8)((color & 0xFF000000) >> 24)); //(Uint8)((color & 0xFF000000) >> 24));
					//SDL_RenderDrawPoint(pRenderer, j, i);

					//SDL_Rect surce;
					//SDL_Rect dis;
					//int w;
					//int h;
					//SDL_QueryTexture(su, NULL, NULL, &w, &h);
					//surce.x = (int)((tex_u)*w);
					//surce.y = (int)((tex_v)*h);
					//
					//dis.x = j;
					//dis.y = i;
					//surce.h = dis.h = 1;
					//surce.w = dis.w = 1;
					//SDL_RenderCopy(pRenderer, texture, &surce, &dis);

					//Game::DrawTexturePixel(pRenderer, surface, , , j, i);
					//pDepthBuffer[i*ScreenWidth() + j] = tex_w;
					//}
					t += tstep;
				}

			}
		}

		dy1 = poly.vertex[2].y - poly.vertex[1].y;
		dx1 = poly.vertex[2].x - poly.vertex[1].x;
		dv1 = poly.uv[2].y - poly.uv[1].y;
		du1 = poly.uv[2].x - poly.uv[1].x;
		dw1 = poly.vertex[2].z - poly.vertex[1].z;

		if (dy1) dax_step = dx1 / (float)abs(dy1);
		if (dy2) dbx_step = dx2 / (float)abs(dy2);

		du1_step = 0, dv1_step = 0;
		if (dy1) du1_step = du1 / (float)abs(dy1);
		if (dy1) dv1_step = dv1 / (float)abs(dy1);
		if (dy1) dw1_step = dw1 / (float)abs(dy1);

		if (dy1)
		{
			for (int i = poly.vertex[1].y; i <= poly.vertex[2].y; i++)
			{
				int ax = poly.vertex[1].x + (float)(i - poly.vertex[1].y) * dax_step;
				int bx = poly.vertex[0].x + (float)(i - poly.vertex[0].y) * dbx_step;

				float tex_su = poly.uv[1].x + (float)(i - poly.vertex[1].y) * du1_step;
				float tex_sv = poly.uv[1].y + (float)(i - poly.vertex[1].y) * dv1_step;
				float tex_sw = poly.vertex[1].z + (float)(i - poly.vertex[1].y) * dw1_step;

				float tex_eu = poly.uv[0].x + (float)(i - poly.vertex[0].y) * du2_step;
				float tex_ev = poly.uv[0].y + (float)(i - poly.vertex[0].y) * dv2_step;
				float tex_ew = poly.vertex[0].z + (float)(i - poly.vertex[0].y) * dw2_step;

				if (ax > bx)
				{
					Swap(ax, bx);
					Swap(tex_su, tex_eu);
					Swap(tex_sv, tex_ev);
					Swap(tex_sw, tex_ew);
				}

				tex_u = tex_su;
				tex_v = tex_sv;
				tex_w = tex_sw;

				float tstep = 1.0f / ((float)(bx - ax));
				float t = 0.0f;

				for (int j = ax; j < bx; j++)
				{
					if (j > m_screenH || i > m_screenW)
						continue;

					tex_u = (1.0f - t) * tex_su + t * tex_eu;
					tex_v = (1.0f - t) * tex_sv + t * tex_ev;
					tex_w = (1.0f - t) * tex_sw + t * tex_ew;

					//if (tex_w > Game::pDepthBuffer[i*Game::width + j])
					//{
					//Uint32 color = getpixel(surface, (int)((tex_u)*surface->clip_rect.w), (int)((tex_v)*surface->clip_rect.h));
					//SDL_SetRenderDrawColor(pRenderer, (Uint8)((color & 0x00FF0000) >> 16),
					//	(Uint8)((color & 0x0000FF00) >> 8),
					//	(Uint8)((color & 0x000000FF)), (Uint8)((color & 0xFF000000) >> 24)); //(Uint8)((color & 0xFF000000) >> 24));
					//SDL_RenderDrawPoint(pRenderer, j, i);

					//SDL_Rect surce;
					//SDL_Rect dis;
					//int w;
					//int h;
					//SDL_QueryTexture(texture, NULL, NULL, &w, &h);
					//surce.x = (int)((tex_u)*w);
					//surce.y = (int)((tex_v)*h);
					//
					//dis.x = j;
					//dis.y = i;
					//surce.h = dis.h = 1;
					//surce.w = dis.w = 1;
					//SDL_RenderCopy(pRenderer, texture, &surce, &dis);

					Uint32 color = GetPixel(pGameObject->GetSurface(), (int)((tex_u)*pGameObject->GetSurface()->clip_rect.w), (int)((tex_v)*pGameObject->GetSurface()->clip_rect.h));
					unsigned char* pixels = (unsigned char*)m_pScreenBuffer->pixels;
					pixels[4 * (j * m_pScreenBuffer->w + i) + 0] = (Uint8)((color & 0xFF000000) >> 24);
					pixels[4 * (j * m_pScreenBuffer->w + i) + 1] = (Uint8)((color & 0x00FF0000) >> 16);
					pixels[4 * (j * m_pScreenBuffer->w + i) + 2] = (Uint8)((color & 0x0000FF00) >> 8);
					pixels[4 * (j * m_pScreenBuffer->w + i) + 3] = (Uint8)((color & 0x000000FF));

					//}
					t += tstep;
				}
			}
		}
	}
}

Uint32 Renderer3D::GetPixel(SDL_Surface * surface, int x, int y)
{
	if (surface == NULL)
		return NULL;
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16 *)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32 *)p;
		break;

	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}

void Renderer3D::DrawPolygon(Vec3 * p, Color color, float b)
{
	Sint16 x[3] = { p[0].x,p[1].x,p[2].x };
	Sint16 y[3] = { p[0].y,p[1].y,p[2].y };
	filledPolygonRGBA(m_pRenderer, x, y, 3, color.r*b, color.g*b, color.b*b, 100);
}
