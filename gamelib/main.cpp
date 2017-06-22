#include "Game.h"
#include <time.h> //srand(time(NULL))

extern Game *game;

INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	srand(time(NULL));

	game = new Game(L"test", 800, 600);

	if (game->Initialize())
		game->Run();

	delete game;

	return 0;
}