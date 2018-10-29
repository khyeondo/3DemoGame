#pragma once

#include "SDL.h"


struct Vec2
{
	float x, y;
};

struct UV
{
	int u, v;
};


class Game
{
private:
	SDL_Window * m_pWindow;
	SDL_Renderer* m_pRenderer;
	bool m_bRunning;

	SDL_Surface* m_image;
	SDL_Texture* m_pTextureImage;

	SDL_Surface* m_pSurface;
	SDL_Texture* m_pTexture;

	SDL_PixelFormat pixelFormat;


	Vec2 vertex[3];
	UV uv[3];

	float fps;
	int startTick;
	int screenWidth;
	int screenHeight;
	char windowTitle[20];


public:
	Game() {}
	~Game() {}

	bool init(const char* title, int xpos, int ypos,
		int width, int height, bool fullscreen);
	void render();
	void update();
	void handleEvents();
	void clean();
	bool running() { return m_bRunning; }

	void swap(int& a, int& b)
	{
		int temp = a;
		a = b;
		b = temp;
	}
	void swap(float& a, float& b)
	{
		float temp = a;
		a = b;
		b = temp;
	}

	Uint32 getpixel(SDL_Surface *surface, int x, int y)
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

	void SurfaceTexturedTriangle(int x1, int y1, float u1, float v1, float w1,
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
					if (j > screenHeight || i > screenWidth)
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

					Uint32 color = getpixel(surface, (int)((tex_u)*surface->clip_rect.w), (int)((tex_v)*surface->clip_rect.h));
					unsigned char* pixels = (unsigned char*)m_pSurface->pixels;
					pixels[4 * (j * m_pSurface->w + i) + 0] = (Uint8)((color & 0xFF000000) >> 24);
					pixels[4 * (j * m_pSurface->w + i) + 1] = (Uint8)((color & 0x00FF0000) >> 16);
					pixels[4 * (j * m_pSurface->w + i) + 2] = (Uint8)((color & 0x0000FF00) >> 8);
					pixels[4 * (j * m_pSurface->w + i) + 3] = (Uint8)((color & 0x000000FF));
				
					//}
					t += tstep;
				}
			}
		}
	}

};