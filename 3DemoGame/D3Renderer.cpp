#include "D3Renderer.h"
#include <algorithm>
D3Renderer* D3Renderer::m_pinst = 0;

void D3Renderer::WorldToCamera(Vec3 & in, Vec3 & out)
{
	out.x = in.x - camera.pos.x;
	out.y = in.y - camera.pos.y;
	out.z = in.z - camera.pos.z;

	out.RotateX(camera.angle.x);
	out.RotateY(camera.angle.y);
	out.RotateZ(camera.angle.z);
}

void D3Renderer::DrawPolygon(SDL_Renderer * pRender, Vec3 * p, Color color, float b)
{
	Sint16 x[3] = { p[0].x,p[1].x,p[2].x };
	Sint16 y[3] = { p[0].y,p[1].y,p[2].y };
	filledPolygonRGBA(pRender, x, y, 3, color.r*b, color.g*b, color.b*b, 255);
}

bool D3Renderer::CullOff(Vec3 a, Vec3 b, Vec3 c)
{
	int S;
	Vec2 Vx, Vy;

	Vx.x = a.x - b.x;
	Vx.y = a.y - b.y;
	Vy.x = c.x - b.x;
	Vy.y = c.y - b.y;

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
	matProj.m[3][3] = 0.0f;

	return true;
}

void D3Renderer::RenderPresent(SDL_Renderer* pRenderer)
{
	//list<_Polygon> listPoly;
	//
	////Polygon의 depth값을 구해서 출력순서를 나열한다.
	//for (auto poly : vecPoly)
	//{
	//	_Polygon temp;
	//	temp.poly = poly;
	//	temp.depth = 
	//		(poly.vertex[0].z + poly.vertex[1].z + poly.vertex[2].z) / 3.0f;
	//
	//	list<_Polygon>::iterator iter;
	//	
	//	if (listPoly.empty())
	//		listPoly.push_back(temp);
	//	else {
	//		for (iter = listPoly.begin(); iter != listPoly.end(); iter++)
	//		{
	//			if ((*iter).depth < temp.depth)
	//			{
	//				listPoly.insert(iter, temp);
	//				break;
	//			}
	//		}
	//		if (iter == listPoly.end())
	//		{
	//			listPoly.push_back(temp);
	//		}
	//	}
	//}

	//for (auto poly : vecPoly)
	//{
	//	poly.depth = 
	//		(poly.vertex[0].z + poly.vertex[1].z + poly.vertex[2].z) / 3.0f;
	//}
	//Polygon의 depth값을 구해서 출력순서를 나열한다.
	sort(vecPoly.begin(), vecPoly.end(), [](Polygon& p1, Polygon& p2)
	{
		float d1 = (p1.vertex[0].z + p1.vertex[1].z + p1.vertex[2].z) / 3.0f;
		float d2 = (p2.vertex[0].z + p2.vertex[1].z + p2.vertex[2].z) / 3.0f;
		return d1 > d2;
	});

	//Camera to Viewer
	for (auto poly : vecPoly)
	{
		Vec3 p[3];
		Vec3 light(0.0f, -1.0f, 0.0f);
		light.Normalize();
		poly.normalVec = Vec3::Cross(poly.vertex[1] - 
			poly.vertex[0], poly.vertex[2] - poly.vertex[0]).Normalize();

		float brightness = poly.normalVec.x * light.x + 
			poly.normalVec.y * light.y + poly.normalVec.z * light.z;

		brightness += 1.f; brightness /= 4.f; 
		brightness += 0.5f;
		bool isDraw = true;
		for (int i = 0; i < 3; i++)
		{
			//MultiplyMatrixVector(poly.vertex[i], p[i], matProj);
			p[i] = poly.vertex[i] * matProj;
			if (p[i].w != 0) {
				p[i].x /= p[i].w; p[i].y /= p[i].w; p[i].z /= p[i].w;
			}
			if ((p[i].z < camera.near || p[i].z > camera.far) ||
				(p[i].x < -1 || p[i].x > 1) ||
				(p[i].y < -1 || p[i].y > 1)) {
				isDraw = false;
				break;
			}

			p[i].x += 1.0f; p[i].y += 1.0f;
			p[i].x *= 0.5f * (float)screenWidth;
			p[i].y *= 0.5f * (float)screenHeight;
		}
		if (!CullOff(p[0], p[1], p[2]) && isDraw)
			DrawPolygon(pRenderer, p, Color(255, 255, 255), brightness);
		//if (!CullOff(p[0], p[1], p[2]) && isDraw) {
		//	SDL_RenderDrawLine(pRenderer, p[0].x, p[0].y,
		//		p[1].x, p[1].y);
		//	SDL_RenderDrawLine(pRenderer, p[1].x, p[1].y,
		//		p[2].x, p[2].y);
		//	SDL_RenderDrawLine(pRenderer, p[2].x, p[2].y,
		//		p[0].x, p[0].y);
		//}
	}

	vecPoly.clear();
}

void D3Renderer::Draw(Mesh & mesh)
{	
	for (auto poly : mesh.polys)
	{	
		Polygon temp;
		for (int i = 0; i < 3; i++) {
			WorldToCamera(poly.vertex[i], temp.vertex[i]);
		}
		bool isDraw = true;
		for (int i = 0; i < 3;  i++)
		{
			if (temp.vertex[i].z < 0)
				isDraw = false;
		}
		if(isDraw == true)
			vecPoly.push_back(temp);
	}
}

void D3Renderer::Draw(D3Object& object)
{
	Mesh mesh;
	//object의 mesh복사
	mesh.polys.assign(object.GetMesh()->polys.begin(),
		object.GetMesh()->polys.end());
	
	//mesh.RotateX(object.GetAngle().x);
	//mesh.RotateY(object.GetAngle().y);
	//mesh.RotateZ(object.GetAngle().z);

	mesh.Translate(Vec3(-object.GetPos().x, -object.GetPos().y,
		-object.GetPos().z));
	Draw(mesh);
}
