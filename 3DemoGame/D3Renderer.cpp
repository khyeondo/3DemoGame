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

	Matrix4X4::MakeRotationX(rotateX, objAngle.x);
	Matrix4X4::MakeRotationY(rotateY, objAngle.y);
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

	//Vec3 rotatedDir = camera.lookDir;

	//rotatedDir *= rotateY;
	//rotatedDir *= rotateX;

	Vec3 target = { 0,0,1 };

	//Matrix4X4 rotate = Matrix4X4::Matrix_MultiplyMatrix(rotateY, rotateX);
	//camera.lookDir = target * rotate;
	//camera.lookDir = camera.lookDir * rotateX;

	Vec3 zaxis = (camera.lookDir - camera.pos).Normalize();
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

		poly.get().vertex[0].w = 1.f;
		poly.get().vertex[1].w = 1.f;
		poly.get().vertex[2].w = 1.f;
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
		DrawPolygon(pRenderer, poly.get().vertex, Color(255, 255, 255), poly.get().brightness);
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








