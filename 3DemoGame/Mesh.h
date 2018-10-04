#pragma once
#include <fstream>
#include <strstream>
#include <vector>
#include "Polygon.h"
using namespace std;
struct Mesh
{
	vector<Polygon> polys;
	
	void RotateX(float angle) {
		for (int i = 0; i < polys.size(); i++)
		{
			polys[i].vertex[0] = polys[i].vertex[0] * Matrix4X4::GetRotateX(angle);
			polys[i].vertex[1] = polys[i].vertex[1] * Matrix4X4::GetRotateX(angle);
			polys[i].vertex[2] = polys[i].vertex[2] * Matrix4X4::GetRotateX(angle);
		}
	}
	void RotateY(float angle) {
		for (int i = 0; i < polys.size(); i++)
		{
			polys[i].vertex[0] = polys[i].vertex[0] * Matrix4X4::GetRotateY(angle);
			polys[i].vertex[1] = polys[i].vertex[1] * Matrix4X4::GetRotateY(angle);
			polys[i].vertex[2] = polys[i].vertex[2] * Matrix4X4::GetRotateY(angle);
		}
	}
	void RotateZ(float angle) {
		for (int i = 0; i < polys.size(); i++)
		{
			polys[i].vertex[0] = polys[i].vertex[0] * Matrix4X4::GetRotateZ(angle);
			polys[i].vertex[1] = polys[i].vertex[1] * Matrix4X4::GetRotateZ(angle);
			polys[i].vertex[2] = polys[i].vertex[2] * Matrix4X4::GetRotateZ(angle);
		}
	}

	void Translate(Vec3 vec)
	{
		for (int i = 0; i < polys.size(); i++)
		{
			polys[i].Translate(vec);
		}
	}

	bool LoadFromObjectFile(string sFilename)
	{
		ifstream f(sFilename);
		if (!f.is_open())
			return false;

		// Local cache of verts
		vector<Vec3> verts;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			strstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				Vec3 v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}

			if (line[0] == 'f')
			{
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				polys.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
			}
		}

		return true;
	}
};