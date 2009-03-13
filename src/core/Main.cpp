#include "Game.hpp"

int main(int argc, char *argv[])
{
    // Lancement du jeu
	Game& game = Game::GetInstance();
	game.Init();
	return game.Run();
}

