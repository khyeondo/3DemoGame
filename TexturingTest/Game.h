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

	SDL_Texture* m_pTexture;
	SDL_Rect m_sourceRectangle;
	SDL_Rect m_destnationRectangle;

	float fps;
	int startTick;
	int screenWidth;
	int screenHeight;
	char windowTitle[20];

	SDL_Surface * m_Surface;
	Uint32* pixels;


	Vec2 vertex[3];
	UV uv[3];


public:
	static float* pDepthBuffer;
	static int width, height;


	Game() {}
	~Game() {}
	bool init(const char* title, int xpos, int ypos,
		int width, int height, bool fullscreen);

	void render();
	void update();
	void handleEvents();
	void clean();
	bool running() { return m_bRunning; }

	static void DrawSurfacePixel(SDL_Renderer* pRenderer, SDL_Surface* pSurface, int tx, int ty,int px,int py);
	static void DrawTexturePixel(SDL_Renderer* pRenderer, SDL_Texture* ,int tx, int ty, int px, int py);
};

