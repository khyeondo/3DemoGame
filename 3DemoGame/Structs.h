#pragma once

struct Vec2
{
public:
	float x, y;

public:
	Vec2() {}
	Vec2(float x, float y) {
		this->x = x; this->y = y;
	}
};

struct Color
{
public:
	Color() {}
	Color(Uint8 r, Uint8 g, Uint8 b) {
		this->r = r; this->g = g; this->b = b;
	}
public:
	Uint8 r, g, b;

	void operator * (float a)
	{
		r *= (Uint8)a;
		g *= (Uint8)a;
		b *= (Uint8)a;
	}
	void operator + (int i)
	{
		r += i;
		g += i;
		b += i;
	}
};
