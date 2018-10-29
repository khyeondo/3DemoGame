
#include <iostream>
#include "Game.h"




bool Game::init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
	{
		m_pWindow = SDL_CreateWindow(title,
			xpos, ypos,
			width, height, SDL_WINDOW_SHOWN && fullscreen);
	}

	if (m_pWindow != 0)
	{
		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
	}
	else
	{

		m_bRunning = false;
		return false;
	}


	uv[0] = { 1,1 };
	uv[1] = { 0,1 };
	uv[2] = { 0,0 };

	vertex[0] = { 500,0 };
	vertex[1] = { 0,0 };
	vertex[2] = { 0,500 };

	m_image = SDL_LoadBMP("assets/rider.bmp");
	m_pTextureImage = SDL_CreateTextureFromSurface(m_pRenderer, m_image);

	screenWidth = width;
	screenHeight = height;
	strcpy_s(windowTitle, 20, (char*)title);
	
	m_bRunning = true;
	////////////////////

	//m_pTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGBA4444, SDL_TEXTUREACCESS_STREAMING, width, height);
	//int w, h;
	//SDL_QueryTexture(m_pTexture, &pixelFormat.format, nullptr, &w, &h);
	////////////////////

	m_pSurface = SDL_CreateRGBSurface(0, screenWidth, screenHeight, 32, 0, 0, 0, 0);

	return true;
}

void Game::render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
	SDL_RenderClear(m_pRenderer);

	SurfaceTexturedTriangle(vertex[0].x, vertex[0].y, uv[0].u, uv[0].v, 0.f,
		vertex[1].x, vertex[1].y, uv[1].u, uv[1].v, 0.f,
		vertex[2].x, vertex[2].y, uv[2].u, uv[2].v, 0.f,
		m_pRenderer, m_image);

	m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_pSurface);

	SDL_Rect rect;
	rect.x = rect.y = 0;
	rect.w = screenWidth;
	rect.h = screenHeight;
	SDL_RenderCopy(m_pRenderer, m_pTexture, &rect, &rect);
	SDL_DestroyTexture(m_pTexture);
	SDL_RenderPresent(m_pRenderer);


	fps = 1000.0f / (float)(SDL_GetTicks() - startTick);
	char cfps[40];

	sprintf_s(cfps, 40, "%s  -  FPS : %0.3f", windowTitle, fps);
	SDL_SetWindowTitle(m_pWindow, cfps);
}


void Game::update()
{

}

void Game::handleEvents()
{
	SDL_Event event;
	startTick = SDL_GetTicks();
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_bRunning = false;
			break;
		default:
			break;
		}
	}
}

void Game::clean()
{
	SDL_FreeSurface(m_image);
	std::cout << "cleaning game\n";
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}
