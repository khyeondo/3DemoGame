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

	m_bRunning = true;

	screenWidth = width;
	screenHeight = height;

	D3Renderer::GetInst()->Init(height, width);

	meshCube.polys = {
		//Cube모양으로 폴리곤을 시계방향으로 정점을 위치시킨다.
		// SOUTH
	{ Vec3(0.0f, 0.0f, 0.0f),    Vec3(0.0f, 1.0f, 0.0f),    Vec3(1.0f, 1.0f, 0.0f) },
	{ Vec3(0.0f, 0.0f, 0.0f),    Vec3(1.0f, 1.0f, 0.0f),    Vec3(1.0f, 0.0f, 0.0f) },

	// EAST                                                      
	{ Vec3(1.0f, 0.0f, 0.0f),   Vec3(1.0f, 1.0f, 0.0f),    Vec3(1.0f, 1.0f, 1.0f) },
	{ Vec3(1.0f, 0.0f, 0.0f),   Vec3(1.0f, 1.0f, 1.0f),    Vec3(1.0f, 0.0f, 1.0f) },

	// NORTH                                                     
	{ Vec3(1.0f, 0.0f, 1.0f),    Vec3(1.0f, 1.0f, 1.0f),    Vec3(0.0f, 1.0f, 1.0f) },
	{ Vec3(1.0f, 0.0f, 1.0f),    Vec3(0.0f, 1.0f, 1.0f),    Vec3(0.0f, 0.0f, 1.0f) },

	// WEST                                                      
	{ Vec3(0.0f, 0.0f, 1.0f),    Vec3(0.0f, 1.0f, 1.0f),    Vec3(0.0f, 1.0f, 0.0f) },
	{ Vec3(0.0f, 0.0f, 1.0f),    Vec3(0.0f, 1.0f, 0.0f),    Vec3(0.0f, 0.0f, 0.0f) },

	// TOP                                                       
	{ Vec3(0.0f, 1.0f, 0.0f),    Vec3(0.0f, 1.0f, 1.0f),    Vec3(1.0f, 1.0f, 1.0f) },
	{ Vec3(0.0f, 1.0f, 0.0f),    Vec3(1.0f, 1.0f, 1.0f),    Vec3(1.0f, 1.0f, 0.0f) },

	// BOTTOM                                                    
	{ Vec3(1.0f, 0.0f, 1.0f),    Vec3(0.0f, 0.0f, 1.0f),    Vec3(0.0f, 0.0f, 0.0f) },
	{ Vec3(1.0f, 0.0f, 1.0f),    Vec3(0.0f, 0.0f, 0.0f),    Vec3(1.0f, 0.0f, 0.0f) },
	};
	mesh.LoadFromObjectFile("teaPot.obj");
	object.SetMesh(&mesh);

	return true;
}

void Game::update()
{
	SetDeltaTime();
}

void Game::render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
	SDL_RenderClear(m_pRenderer);
	//SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);

	//D3Renderer::GetInst()->Draw(meshCube);
	
	object.angle += Vec3(0.05f, 0.05f, 0.05f);
	//object.angle = Vec3(14.f, 0.01f, 0.01f);
	//object.pos = Vec3(14.f, 0.01f, 100.f);
	D3Renderer::GetInst()->Draw(object);

	D3Renderer::GetInst()->RenderPresent(m_pRenderer);
	SDL_RenderPresent(m_pRenderer);
}

void Game::handleEvents()
{
	SDL_Event event;


	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_bRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_UP)
			{
				D3Renderer::GetInst()->camera.angle.x -= 0.03f;
			}
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				D3Renderer::GetInst()->camera.angle.x += 0.03f;
			}
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				D3Renderer::GetInst()->camera.angle.y -= 0.03f;
			}
			if (event.key.keysym.sym == SDLK_RIGHT)
			{
				D3Renderer::GetInst()->camera.angle.y += 0.03f;
			}

			if (event.key.keysym.sym == SDLK_w)
			{
				D3Renderer::GetInst()->camera.pos.z += 10.0f*deltaTime;
			}
			if (event.key.keysym.sym == SDLK_s)
			{
				D3Renderer::GetInst()->camera.pos.z -= 10.0f*deltaTime;
			}
			if (event.key.keysym.sym == SDLK_a)
			{
				D3Renderer::GetInst()->camera.pos.x -= 10.0f*deltaTime;
			}
			if (event.key.keysym.sym == SDLK_d)
			{
				D3Renderer::GetInst()->camera.pos.x += 10.0f*deltaTime;
			}
			if (event.key.keysym.sym == SDLK_z)
			{
				D3Renderer::GetInst()->camera.pos.y -= 10.0f*deltaTime;
			}
			if (event.key.keysym.sym == SDLK_x)
			{
				D3Renderer::GetInst()->camera.pos.y += 10.0f*deltaTime ;
			}
			break;
		//case SDL_MOUSEMOTION:
		//	cam->SetAngleX(cam->GetAngleX() + (event.motion.y - prePos.y)*0.003f);
		//	cam->SetAngleY(cam->GetAngleY() + (event.motion.x - prePos.x)*0.003f);
		//	prePos.x = event.motion.x;
		//	prePos.y = event.motion.y;
		//	break;
		default:
			break;
		}
	}
}

void Game::clean()
{
	std::cout << "cleaning game\n";
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}

void Game::SetDeltaTime()
{
	LAST = NOW;
	NOW = SDL_GetPerformanceCounter();

	deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());
}
