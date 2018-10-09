#include "Game.h"


int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game::GetInst()->init("3DGameDemo", 100, 100, 1024, 720, false);
	while (Game::GetInst()->running())
	{
		Game::GetInst()->handleEvents();
		Game::GetInst()->update();
		Game::GetInst()->render();
	}
	//int* a = new int;
	Game::GetInst()->clean();
	Game::GetInst()->DeleteSingle();
	
	return 0;
}