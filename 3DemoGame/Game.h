#pragma once

#include "defulte.h"
#include "D3Renderer.h"

class Game
{
private:
	SDL_Window * m_pWindow;
	SDL_Renderer* m_pRenderer;
	bool m_bRunning;

	Mesh meshCube;
	Mesh ship;
	int screenWidth, screenHeight;
	float fTheta = 1;


	D3Object object;
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

};