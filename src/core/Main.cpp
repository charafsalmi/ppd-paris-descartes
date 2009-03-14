#include "Game.hpp"


int main(int argc, char *argv[])
{
	Game& game = Game::GetInstance();
	game.Init();
	return game.Run();
}

