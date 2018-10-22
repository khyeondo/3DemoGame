#pragma once

#include "defulte.h"
#include "D3Renderer.h"

class Game
{
#pragma region Single
private:
	static Game * m_pinst;
public:
	static Game * GetInst() {
		if (m_pinst == 0)
			m_pinst = new Game();
		return m_pinst;
	}
	static void DeleteSingle() {
		if (m_pinst != 0)
			delete m_pinst;
		m_pinst = 0;
	}
#pragma endregion

private:
	SDL_Surface * m_Surface;
	Uint32* pixels;

	SDL_Window * m_pWindow;
	SDL_Renderer* m_pRenderer;

	bool m_bRunning;

	float fps;
	int startTick;

	char windowTitle[20];
	int screenWidth, screenHeight;

	bool isKeyHolding[1000] = {false};

	Mesh mesh;

	D3Object object;
private:
	Game() {}

public:
	~Game() {}
	bool init(const char* title, int xpos, int ypos,
		int width, int height, bool fullscreen);

	void start();
	void handleEvents();
	void update();
	void render();
	void clean();
	bool running() { return m_bRunning; }
	
};