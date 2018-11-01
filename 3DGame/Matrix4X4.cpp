
#include "Matrix4X4.h"
#include "Vec3.h"

void Matrix4X4::MakeProjectionMatrix(Matrix4X4 & mat, int fov, int near, int far, int screenW, int screenH)
{
	float aspect = (float)screenH / (float)screenW;
	float yScale = 1.0f / tanf((fov*0.5f) / 180.0f* 3.141592f);
	float xScale = yScale / aspect;
	//참고:https://docs.microsoft.com/en-us/windows/desktop/direct3d9/d3dxmatrixperspectivefovlh
	/*
	xScale     0          0               0
	0        yScale       0               0
	0          0       zf/(zf-zn)         1
	0          0       -zn*zf/(zf-zn)     0
	*/
	mat.m[0][0] = xScale;
	mat.m[1][1] = yScale;
	mat.m[2][2] = far / (far - near);
	mat.m[2][3] = 1.f;
	mat.m[3][2] = (-near * far) / (far - near);

	//float screenRatio = (float)screenH / (float)screenW;
	//float fovTan = 1.0f / tanf(fov*0.5f / 180.0f* 3.141592f);
	///*
	//	ratio*fov	0			0			0
	//	0			fov			0			0
	//	0			0			f/(f-n)		-(f*n)/(f-n)
	//	0			0			1			0
	//*/
	//
	//mat.m[0][0] = screenRatio * fovTan;
	//mat.m[1][1] = fovTan;
	//mat.m[2][2] = far / (far - near);
	//mat.m[2][3] = (-far * near) / (far - near);
	//mat.m[3][2] = 1.f;
}

void Matrix4X4::MakeLookAtMatrix(Matrix4X4 & mat, Vec3 & pos, Vec3 & lookAt, Vec3 up)
{
	//참고:https://docs.microsoft.com/en-us/windows/desktop/direct3d9/d3dxmatrixlookatlh
	Vec3 z = (lookAt - pos).Normalize();
	Vec3 x = Vec3::CrossProduct(up, z).Normalize();
	Vec3 y = Vec3::CrossProduct(z, x);

	mat.m[0][0] = x.x; mat.m[0][1] = y.x; mat.m[0][2] = z.x; mat.m[0][3] = 0.f;
	mat.m[1][0] = x.y; mat.m[1][1] = y.y; mat.m[1][2] = z.y; mat.m[1][3] = 0.f;
	mat.m[2][0] = x.z; mat.m[2][1] = y.z; mat.m[2][2] = z.z; mat.m[2][3] = 0.f;
	mat.m[3][0] = -Vec3::DotProduct(x, pos); mat.m[3][1] = -Vec3::DotProduct(y, pos);
	mat.m[3][2] = -Vec3::DotProduct(z, pos); mat.m[3][3] = 1.f;

	//mat.m[0][0] = x.x; mat.m[1][0] = y.x; mat.m[2][0] = z.x; mat.m[3][0] = 0.f;
	//mat.m[0][1] = x.y; mat.m[1][1] = y.y; mat.m[2][1] = z.y; mat.m[3][1] = 0.f;
	//mat.m[0][2] = x.z; mat.m[1][2] = y.z; mat.m[2][2] = z.z; mat.m[3][2] = 0.f;
	//mat.m[0][3] = -Vec3::DotProduct(x, pos); mat.m[1][3] = -Vec3::DotProduct(y, pos);
	//mat.m[2][3] = -Vec3::DotProduct(z, pos); mat.m[3][3] = 1.f;
}

void Matrix4X4::MakeViewPortMatrix(Matrix4X4 & mat, int screenW, int screenH)
{
	mat.m[0][0] = screenW / 2.f;
	mat.m[1][1] = -screenH / 2.f;
	//mat.m[2][2] = 1.f;
	//mat.m[3][3] = 1.f;
	mat.m[3][0] = screenW / 2.f;
	mat.m[3][1] = screenH / 2.f;
}

void Matrix4X4::MakeScalingMatrix(Matrix4X4 & mat, float sizeX, float sizeY, float sizeZ)
{
	mat.m[0][0] = sizeX;
	mat.m[1][1] = sizeY;
	mat.m[2][2] = sizeZ;
	mat.m[3][3] = 1.f;
}

void Matrix4X4::MakeRotationX(Matrix4X4 & mat, float angle)
{
	/*
	1		0		0		0
	0		cos		sin		0
	0	    -sin	cos		0
	0		0		0		1
	*/
	mat.m[0][0] = 1;
	mat.m[1][1] = cosf(angle);
	mat.m[1][2] = sinf(angle);
	mat.m[2][1] = -sinf(angle);
	mat.m[2][2] = cosf(angle);
	mat.m[3][3] = 1;
}

void Matrix4X4::MakeRotationY(Matrix4X4 & mat, float angle)
{
	/*
	cos		0		sin		0
	0		1		0		0
	-sin	0 		cos		0
	0		0		0		1
	*/
	mat.m[0][0] = cosf(angle);
	mat.m[0][2] = sinf(angle);
	mat.m[2][0] = -sinf(angle);
	mat.m[1][1] = 1.0f;
	mat.m[2][2] = cosf(angle);
	mat.m[3][3] = 1.0f;
}

void Matrix4X4::MakeRotationZ(Matrix4X4 & mat, float angle)
{
	/*
	cos		sin		0		0
	-sin	cos		0		0
	0	    0		1		0
	0		0		0		1
	*/
	mat.m[0][0] = cosf(angle);
	mat.m[0][1] = sinf(angle);
	mat.m[1][0] = -sinf(angle);
	mat.m[1][1] = cos(angle);
	mat.m[2][2] = 1;
	mat.m[3][3] = 1;
}

void Matrix4X4::MatrixMultiplyMatrix(Matrix4X4 & in1, Matrix4X4 & in2, Matrix4X4 & out)
{
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			out.m[r][c] = in2.m[r][0] * in1.m[0][c] + in2.m[r][1] * in1.m[1][c] + in2.m[r][2] * in1.m[2][c] + in2.m[r][3] * in1.m[3][c];
}