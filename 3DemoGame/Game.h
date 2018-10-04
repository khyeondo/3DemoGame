#pragma once

#include "defulte.h"
#include "D3Renderer.h"

class Game
{
private:
	SDL_Window * m_pWindow;
	SDL_Renderer* m_pRenderer;

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;

	double deltaTime = 0;
	bool m_bRunning;
	int screenWidth, screenHeight;

	Mesh meshCube;
	Mesh mesh;
	float fTheta = 1;

	D3Object object;

	Vec3 x = { 5,0,0 };
	Vec3 y = { 0,5,0 };
	Vec3 z = { 0,0,5 };
public:
	Game() {}
	~Game() {}
	bool init(const char* title, int xpos, int ypos,
		int width, int height, bool fullscreen);

	void update();
	void render();
	void handleEvents();
	void clean();
	bool running() { return m_bRunning; }
	
	void SetDeltaTime();
};