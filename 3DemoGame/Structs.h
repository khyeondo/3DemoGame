#pragma once

typedef unsigned char byte;

struct Vec2
{
	float x, y;
};

struct Color
{
public:
	Color() {}
	Color(byte r, byte g, byte b) {
		this->r = r; this->g = g; this->b = b;
	}
public:
	byte r, g, b;

	void operator * (float a)
	{
		if (a >= 0 && a <= 1)
		{
			r *= (byte)a;
			g *= (byte)a;
			b *= (byte)a;
		}
	}
};
