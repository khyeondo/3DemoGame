#include "D3Renderer.h"
#include <algorithm>

D3Renderer* D3Renderer::m_pinst = 0;

void D3Renderer::RenderingPipeline(SDL_Renderer* pRenderer)
{
	WorldToCamera();
	CameraToViewer();
	Culling();
	Shading();
	DepthSort();
	ViewerToViewport();
	OutPut(pRenderer);

	vecPoly.clear();
	vecInCameraPoly.clear();
	vecCulledPoly.clear();
}

void D3Renderer::LocalToWorld(Mesh& mesh, Vec3 objPos, Vec3 objAngle)
{
	Matrix4X4 rotateX;
	Matrix4X4 rotateY;
	Matrix4X4 rotateZ;

	Matrix4X4::MakeRotationY(rotateY, objAngle.y);
	Matrix4X4::MakeRotationX(rotateX, objAngle.x);
	Matrix4X4::MakeRotationZ(rotateZ, objAngle.z);

	for (auto& poly : mesh.polys)
	{
		for (int i = 0; i < 3; i++)
		{
			poly.vertex[i] -= objPos;

			poly.vertex[i] *= rotateX;
			poly.vertex[i] *= rotateY;
			poly.vertex[i] *= rotateZ;
		}
		poly.normalVec = Vec3::CrossProduct(
			poly.vertex[1] - poly.vertex[0],
			poly.vertex[2] - poly.vertex[0]).Normalize();

		vecPoly.push_back(poly);
	}

}

void D3Renderer::WorldToCamera()
{	
	//Vec3 up = { 0,1,0 };
	//Vec3 target = { 0,0,1 };
	//
	//Matrix4X4 rotateX;
	//Matrix4X4 rotateY;
	//Matrix4X4::MakeRotationX(rotateX, camera.angle.x);
	//Matrix4X4::MakeRotationY(rotateY, camera.angle.y);
	//target *= rotateX;
	//target *= rotateY;
	//
	//camera.lookDir = target;
	//target = camera.pos + camera.lookDir;
	//
	//Vec3 forward = target - camera.pos;
	//forward.Normalize();
	//
	//Vec3 a = forward * Vec3::DotProduct(up, forward);
	//up -= a;
	//up.Normalize();
	//
	//Vec3 right = Vec3::CrossProduct(up, forward);
	//
	////Vec3 right = { camera.forward.z,camera.forward.y,-camera.forward.x };
	//
	//matLootAt.m[0][0] = right.x;			matLootAt.m[0][1] = right.y;			matLootAt.m[0][2] = right.z;			matLootAt.m[0][3] = 0.0f;
	//matLootAt.m[1][0] = up.x;				matLootAt.m[1][1] = up.y;				matLootAt.m[1][2] = up.z;				matLootAt.m[1][3] = 0.0f;
	//matLootAt.m[2][0] = forward.x;		matLootAt.m[2][1] = forward.y;			matLootAt.m[2][2] = forward.z;			matLootAt.m[2][3] = 0.0f;
	//matLootAt.m[3][0] = camera.pos.x;		matLootAt.m[3][1] = camera.pos.y;		matLootAt.m[3][2] = camera.pos.z;		matLootAt.m[3][3] = 1.0f;
	//
	//Matrix4X4 matrix;
	//matrix.m[0][0] = matLootAt.m[0][0]; matrix.m[0][1] = matLootAt.m[1][0]; matrix.m[0][2] = matLootAt.m[2][0]; matrix.m[0][3] = 0.0f;
	//matrix.m[1][0] = matLootAt.m[0][1]; matrix.m[1][1] = matLootAt.m[1][1]; matrix.m[1][2] = matLootAt.m[2][1]; matrix.m[1][3] = 0.0f;
	//matrix.m[2][0] = matLootAt.m[0][2]; matrix.m[2][1] = matLootAt.m[1][2]; matrix.m[2][2] = matLootAt.m[2][2]; matrix.m[2][3] = 0.0f;
	//matrix.m[3][0] = -(matLootAt.m[3][0] * matrix.m[0][0] + matLootAt.m[3][1] * matrix.m[1][0] + matLootAt.m[3][2] * matrix.m[2][0]);
	//matrix.m[3][1] = -(matLootAt.m[3][0] * matrix.m[0][1] + matLootAt.m[3][1] * matrix.m[1][1] + matLootAt.m[3][2] * matrix.m[2][1]);
	//matrix.m[3][2] = -(matLootAt.m[3][0] * matrix.m[0][2] + matLootAt.m[3][1] * matrix.m[1][2] + matLootAt.m[3][2] * matrix.m[2][2]);
	//matrix.m[3][3] = 1.0f;

	Matrix4X4 matrixLootAt;

	Vec3 up = { 0,1,0 };

	Matrix4X4 rotateX;
	Matrix4X4 rotateY;
	Matrix4X4::MakeRotationX(rotateX, camera.angle.x);
	Matrix4X4::MakeRotationY(rotateY, camera.angle.y);

	Vec3 rotatedDir = camera.lookDir - camera.pos;

	//rotatedDir *= rotateY;
	//rotatedDir *= rotateX;

	Vec3 target = { 0,0,1 };

	Matrix4X4 rotate;
	Matrix4X4::Matrix_MultiplyMatrix(rotateY, rotateX, rotate);
	rotatedDir *= rotate;

	Vec3 zaxis = (rotatedDir).Normalize();
	Vec3 xaxis = Vec3::CrossProduct(up, zaxis).Normalize();
	Vec3 yaxis = Vec3::CrossProduct(zaxis, xaxis);

	matrixLootAt.m[0][0] = xaxis.x;  matrixLootAt.m[0][1] = yaxis.x;	  matrixLootAt.m[0][2] = zaxis.x;	   matrixLootAt.m[0][3] = 0;
	matrixLootAt.m[1][0] = xaxis.y;	 matrixLootAt.m[1][1] = yaxis.y;	  matrixLootAt.m[1][2] = zaxis.y;	   matrixLootAt.m[1][3] = 0;
	matrixLootAt.m[2][0] = xaxis.z;	 matrixLootAt.m[2][1] = yaxis.z;	  matrixLootAt.m[2][2] = zaxis.z;	   matrixLootAt.m[2][3] = 0;
	matrixLootAt.m[3][0] = -Vec3::DotProduct(xaxis, camera.pos);    matrixLootAt.m[3][1] = -Vec3::DotProduct(yaxis, camera.pos);
	matrixLootAt.m[3][2] = -Vec3::DotProduct(zaxis, camera.pos);    matrixLootAt.m[3][3] = 1;

	for (auto& poly : vecPoly)
	{
		poly.vertex[0] *= matrixLootAt;
		poly.vertex[1] *= matrixLootAt;
		poly.vertex[2] *= matrixLootAt;

		if (!(poly.vertex[0].z < 0 || poly.vertex[1].z < 0 || poly.vertex[1].z < 0))//z가 양수일때만 
			vecInCameraPoly.push_back(poly);
	}
}

void D3Renderer::CameraToViewer()
{
	for (auto& poly : vecInCameraPoly)
	{
		poly.get().vertex[0] *= matProj;
		poly.get().vertex[1] *= matProj;
		poly.get().vertex[2] *= matProj;

		poly.get().vertex[0].x /= poly.get().vertex[0].w;
		poly.get().vertex[0].y /= poly.get().vertex[0].w;
		poly.get().vertex[0].z /= poly.get().vertex[0].w;

		poly.get().vertex[1].x /= poly.get().vertex[1].w;
		poly.get().vertex[1].y /= poly.get().vertex[1].w;
		poly.get().vertex[1].z /= poly.get().vertex[1].w;

		poly.get().vertex[2].x /= poly.get().vertex[2].w;
		poly.get().vertex[2].y /= poly.get().vertex[2].w;
		poly.get().vertex[2].z /= poly.get().vertex[2].w;

		//poly.get().vertex[0].w = 1.f;
		//poly.get().vertex[1].w = 1.f;
		//poly.get().vertex[2].w = 1.f;
	}
}

void D3Renderer::Culling()
{
	bool isOut;
	for (auto& poly : vecInCameraPoly)
	{
		isOut = true;
		for (int i = 0; i < 3; i++)
		{
			if ((poly.get().vertex[i].z < camera.near || poly.get().vertex[i].z > camera.far) ||
				(poly.get().vertex[i].x < -1 || poly.get().vertex[i].x > 1) ||
				(poly.get().vertex[i].y < -1 || poly.get().vertex[i].y > 1))
			{
				isOut = false;
			}
		}
		if (!CullOff(poly.get()) && isOut)
		{
			vecCulledPoly.push_back(poly.get());
		}
	}
}

void D3Renderer::Shading()
{
	for (auto& poly : vecCulledPoly)
	{
		poly.get().brightness = poly.get().normalVec.x * light.x +poly.get().normalVec.y * light.y + poly.get().normalVec.z * light.z;
		poly.get().brightness *= -1;
		if (poly.get().brightness < 0)
			poly.get().brightness = 0;
		//poly.get().brightness += 1.f;
		//poly.get().brightness *= (1.f / 2.f);
	}
}

void D3Renderer::DepthSort()
{
	sort(vecCulledPoly.begin(), vecCulledPoly.end(), [](Polygon& p1, Polygon& p2)
	{
		float d1 = (p1.vertex[0].z + p1.vertex[1].z + p1.vertex[2].z) / 3.0f;
		float d2 = (p2.vertex[0].z + p2.vertex[1].z + p2.vertex[2].z) / 3.0f;
		return d1 > d2;
	});
}

void D3Renderer::ViewerToViewport()
{
	for (auto& poly : vecCulledPoly)
	{
		poly.get().vertex[0].w = 1.f;
		poly.get().vertex[1].w = 1.f;
		poly.get().vertex[2].w = 1.f;

		poly.get().vertex[0] *= matViewport;
		poly.get().vertex[1] *= matViewport;
		poly.get().vertex[2] *= matViewport;
	}
}

void D3Renderer::OutPut(SDL_Renderer* pRenderer)
{
	for (auto& poly : vecCulledPoly)
	{
		TexturedTriangle(poly.get().vertex[0].x, poly.get().vertex[0].y, poly.get().uv[0].x, poly.get().uv[0].y, poly.get().vertex[0].w,
			poly.get().vertex[1].x, poly.get().vertex[1].y, poly.get().uv[1].x, poly.get().uv[1].y, poly.get().vertex[0].w,
			poly.get().vertex[2].x, poly.get().vertex[2].y, poly.get().uv[2].x, poly.get().uv[2].y, poly.get().vertex[0].w,
			pRenderer, surface);
		//DrawPolygon(pRenderer, poly.get().vertex, Color(255, 255, 255), poly.get().brightness);
	}
}

void D3Renderer::DrawPolygon(SDL_Renderer * pRenderer, Vec3 * p, Color color, float b)
{
	Sint16 x[3] = { p[0].x,p[1].x,p[2].x };
	Sint16 y[3] = { p[0].y,p[1].y,p[2].y };
	filledPolygonRGBA(pRenderer, x, y, 3, color.r*b, color.g*b, color.b*b, 255);
}

bool D3Renderer::CullOff(Polygon& poly)
{
	float S;
	Vec2 Vx, Vy;

	Vx.x = poly.vertex[0].x - poly.vertex[1].x;
	Vx.y = poly.vertex[0].y - poly.vertex[1].y;
	Vy.x = poly.vertex[2].x - poly.vertex[1].x;
	Vy.y = poly.vertex[2].y - poly.vertex[1].y;

	//시계방향
	S = Vx.x*Vy.y - Vx.y*Vy.x;
	if (S<0)
		return true;
	else
		return false;
}

bool D3Renderer::Init(int screenH, int screenW)
{
	screenWidth = screenW;
	screenHeight = screenH;

	pDepthBuffer = new float[screenH * screenW];

	for (int i = 0; i < screenH*screenW; i++)
		pDepthBuffer[i] = 0;

	camera.near = 0.1f;
	camera.far = 1000.0f;
	camera.fov = 90.0f;

	float screenRatio = (float)screenH / (float)screenW;
	float fovTan = 1.0f / tanf(camera.fov*0.5f/180.0f* 3.141592f);

	matProj.m[0][0] = screenRatio*fovTan;
	matProj.m[1][1] = fovTan;
	matProj.m[2][2] = camera.far / (camera.far - camera.near);
	matProj.m[3][2] = (-camera.far * camera.near) / (camera.far - camera.near);
	matProj.m[2][3] = 1.f;

	matViewport.m[0][0] = screenW / 2.f;
	matViewport.m[1][1] = -screenH / 2.f;
	matViewport.m[3][0] = screenW / 2.f;
	matViewport.m[3][1] = screenH / 2.f;

	light.Normalize();

	return true;
}

void D3Renderer::Draw(D3Object& object)
{
	Mesh mesh;

	//object의 mesh복사
	mesh.polys.assign(object.GetMesh()->polys.begin(),
		object.GetMesh()->polys.end());
	
	LocalToWorld(mesh, object.GetPos(), object.GetAngle());
}

void D3Renderer::RenderPresent(SDL_Renderer* pRenderer)
{
	RenderingPipeline(pRenderer);
}

///////////////////////////////////////////////////////////////////////////////////////////

Uint32 D3Renderer::GetPixel(SDL_Surface *surface, int x, int y)
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

void D3Renderer::TexturedTriangle(int x1, int y1, float u1, float v1, float w1,
	int x2, int y2, float u2, float v2, float w2,
	int x3, int y3, float u3, float v3, float w3,
	SDL_Renderer* pRenderer, SDL_Surface *surface)
{
	if (y2 < y1)
	{
		swap(y1, y2);
		swap(x1, x2);
		swap(u1, u2);
		swap(v1, v2);
		swap(w1, w2);
	}

	if (y3 < y1)
	{
		swap(y1, y3);
		swap(x1, x3);
		swap(u1, u3);
		swap(v1, v3);
		swap(w1, w3);
	}

	if (y3 < y2)
	{
		swap(y2, y3);
		swap(x2, x3);
		swap(u2, u3);
		swap(v2, v3);
		swap(w2, w3);
	}

	int dy1 = y2 - y1;
	int dx1 = x2 - x1;
	float dv1 = v2 - v1;
	float du1 = u2 - u1;
	float dw1 = w2 - w1;

	int dy2 = y3 - y1;
	int dx2 = x3 - x1;
	float dv2 = v3 - v1;
	float du2 = u3 - u1;
	float dw2 = w3 - w1;

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
		for (int i = y1; i <= y2; i++)
		{
			int ax = x1 + (float)(i - y1) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			float tex_su = u1 + (float)(i - y1) * du1_step;
			float tex_sv = v1 + (float)(i - y1) * dv1_step;
			float tex_sw = w1 + (float)(i - y1) * dw1_step;

			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;

			if (ax > bx)
			{
				swap(ax, bx);
				swap(tex_su, tex_eu);
				swap(tex_sv, tex_ev);
				swap(tex_sw, tex_ew);
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
				if (tex_w > pDepthBuffer[i*screenWidth + j])
				{
				//Draw(j, i, tex->SampleGlyph(tex_u / tex_w, tex_v / tex_w), tex->SampleColour(tex_u / tex_w, tex_v / tex_w));
				Uint32 color = GetPixel(surface, (int)((tex_u)*surface->clip_rect.w/tex_w), (int)((tex_v)*surface->clip_rect.h/tex_w));
				SDL_SetRenderDrawColor(pRenderer, (Uint8)((color & 0x00FF0000) >> 16),
					(Uint8)((color & 0x0000FF00) >> 8),
					(Uint8)((color & 0x000000FF)), (Uint8)((color & 0xFF000000) >> 24)); //(Uint8)((color & 0xFF000000) >> 24));
				SDL_RenderDrawPoint(pRenderer, j, i);


				//Game::DrawTexturePixel(pRenderer, surface, , , j, i);
				pDepthBuffer[i*screenWidth + j] = tex_w;
				}
				t += tstep;
			}

		}
	}

	dy1 = y3 - y2;
	dx1 = x3 - x2;
	dv1 = v3 - v2;
	du1 = u3 - u2;
	dw1 = w3 - w2;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);

	du1_step = 0, dv1_step = 0;
	if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1);
	if (dy1) dw1_step = dw1 / (float)abs(dy1);

	if (dy1)
	{
		for (int i = y2; i <= y3; i++)
		{
			int ax = x2 + (float)(i - y2) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			float tex_su = u2 + (float)(i - y2) * du1_step;
			float tex_sv = v2 + (float)(i - y2) * dv1_step;
			float tex_sw = w2 + (float)(i - y2) * dw1_step;

			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;

			if (ax > bx)
			{
				swap(ax, bx);
				swap(tex_su, tex_eu);
				swap(tex_sv, tex_ev);
				swap(tex_sw, tex_ew);
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

				//if (tex_w > Game::pDepthBuffer[i*Game::width + j])
				//{
				Uint32 color = GetPixel(surface, (int)((tex_u)*surface->clip_rect.w), (int)((tex_v)*surface->clip_rect.h));
				SDL_SetRenderDrawColor(pRenderer, (Uint8)((color & 0x00FF0000) >> 16),
					(Uint8)((color & 0x0000FF00) >> 8),
					(Uint8)((color & 0x000000FF)), (Uint8)((color & 0xFF000000) >> 24)); //(Uint8)((color & 0xFF000000) >> 24));
				SDL_RenderDrawPoint(pRenderer, j, i);

				//}
				t += tstep;
			}
		}
	}
}