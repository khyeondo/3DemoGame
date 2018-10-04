#pragma once
#include <fstream>
#include <strstream>
#include <vector>
#include "Polygon.h"
using namespace std;
struct Mesh
{
	vector<Polygon> polys;
	
	void operator *= (Matrix4X4& mat)
	{
		//int size = polys.size();
		//for (int i = 0; i < size; i++)
		//{
		//	Polygon& poly = polys[i];
		//	poly.vertex[0] *= mat;
		//	poly.vertex[1] *= mat;
		//	poly.vertex[2] *= mat;
		//}
		for (auto& poly : polys)
		{
			poly.vertex[0] *= mat;
			poly.vertex[1] *= mat;
			poly.vertex[2] *= mat;
		}
	}

	void Translate(Vec3 vec)
	{
		int size = polys.size();
		for (int i = 0; i < size; i++)
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