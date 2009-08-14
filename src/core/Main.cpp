#include "Game.hpp"


int main()
{
	Game& game = Game::GetInstance();
	game.Init();
	return game.Run();
}

