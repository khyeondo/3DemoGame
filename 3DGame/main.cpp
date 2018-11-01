#include "Game.h"


int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	const int FPS = 60;
	const int DELAY_TIME = 1000.0f / FPS;

	Game::GetInst()->init("3DGame", 100, 100, 1024, 720, false);
	Game::GetInst()->start();

	Uint32 frameStart, frameTime;
	while (Game::GetInst()->running())
	{
		frameStart = SDL_GetTicks();
		Game::GetInst()->handleEvents();
		Game::GetInst()->update();
		Game::GetInst()->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameTime < DELAY_TIME)
		{
			SDL_Delay((int)(DELAY_TIME - frameTime));
		}
	}
	
	Game::GetInst()->clean();
	Game::GetInst()->DeleteSingle();
	
	return 0;
}