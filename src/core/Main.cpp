#include "Game.hpp"

#include "../misc/LogDebug.hpp"

int main(int argc, char *argv[])
{
    // Initialisation du Logger
    Log::SetLogger(new LogDebug());

    // Lancement du jeu
	Game& game = Game::GetInstance();
	game.Init();
	return game.Run();
}

