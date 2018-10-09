#include <iostream>
#include "Game.h"

Game* Game::m_pinst = 0;

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

	m_bRunning = true;

	screenWidth = width;
	screenHeight = height;

	strcpy_s(windowTitle,20 ,(char*)title);

	D3Renderer::GetInst()->Init(height, width);

	mesh.LoadFromObjectFile("teaPot.obj");
	object.SetMesh(&mesh);
	return true;
}


void Game::start()
{
}

void Game::handleEvents()
{
	startTick = SDL_GetTicks();
	SDL_Event event;

	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			isKeyHolding[event.key.keysym.sym % 107374100] = true;
			break;
		case SDL_KEYUP:
			isKeyHolding[event.key.keysym.sym % 107374100] = false;
			break;
		case SDL_QUIT:
			m_bRunning = false;
			break;
		default:
			break;
		}
	}
}

void Game::update()
{
	//object.angle += Vec3(0.05f, 0.05f, 0.05f);
	Matrix4X4 rotateZ;
	Matrix4X4::MakeRotationZ(rotateZ, 0.02f);
	D3Renderer::GetInst()->light *= rotateZ;

	if (isKeyHolding[SDLK_a])
	{
		D3Renderer::GetInst()->camera.pos -= D3Renderer::GetInst()->camera.GetRight();
	}
	if (isKeyHolding[SDLK_d])
	{
		D3Renderer::GetInst()->camera.pos += D3Renderer::GetInst()->camera.GetRight();
	}
	if (isKeyHolding[SDLK_w])
	{
		D3Renderer::GetInst()->camera.pos += D3Renderer::GetInst()->camera.lookDir;
	}
	if (isKeyHolding[SDLK_s])
	{
		D3Renderer::GetInst()->camera.pos -= D3Renderer::GetInst()->camera.lookDir;
	}
	if (isKeyHolding[SDLK_z])
	{
		D3Renderer::GetInst()->camera.pos.y += 3.0f;
	}
	if (isKeyHolding[SDLK_x])
	{
		D3Renderer::GetInst()->camera.pos.y -= 3.0f;
	}
	if (isKeyHolding[SDLK_UP % 107374100])
	{
		D3Renderer::GetInst()->camera.angle.x -= 0.1f;
	}
	if (isKeyHolding[SDLK_DOWN % 107374100])
	{
		D3Renderer::GetInst()->camera.angle.x += 0.1f;
	}
	if (isKeyHolding[SDLK_LEFT % 107374100])
	{
		D3Renderer::GetInst()->camera.angle.y += 0.1f;
	}
	if (isKeyHolding[SDLK_RIGHT % 107374100])
	{
		D3Renderer::GetInst()->camera.angle.y -= 0.1f;
	}
}

void Game::render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
	SDL_RenderClear(m_pRenderer);
	//SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);

	//D3Renderer::GetInst()->Draw(meshCube);
	
	//
	//object.angle = Vec3(14.f, 0.01f, 0.01f);
	//object.pos = Vec3(14.f, 0.01f, 100.f);
	D3Renderer::GetInst()->Draw(object);

	D3Renderer::GetInst()->RenderPresent(m_pRenderer);
	SDL_RenderPresent(m_pRenderer);

	fps = 1000.0f / (float)(SDL_GetTicks() - startTick);
	char cfps[40];

	sprintf_s(cfps,40, "%s  -  FPS : %0.3f", windowTitle , fps);
	SDL_SetWindowTitle(m_pWindow, cfps);
}

void Game::clean()
{
	std::cout << "cleaning game\n";

	D3Renderer::DeleteSingle();

	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}
