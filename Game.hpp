#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "Zone.hpp"
#include "Player.hpp"
#include "ControlPanel.hpp"


class Game
{
public:
	static Game& GetInstance();

	/**
	 * Lancer l'application
	 */
	void Run();

	enum Direction
	{
		UP, DOWN, LEFT, RIGHT
	};

	/**
	 * Demander un changement de zone
	 * @param[in] dir: direction souhaitée
	 */
	void ChangeZone(Direction dir);
	
	/*inline sf::RenderWindow& GetApp() const
	{
		return app_;
	}*/
	
	void ChangeZone(const char* zone);
	
	inline Zone* GetZone()
	{
		return active_zone_;
	}

private:
	Game();
	Game(const Game&);
	~Game();

	void SetMusic(short val);

	enum
	{
		// dimensions du jeu en nombre de zones
		GAME_WIDTH = 3, GAME_HEIGHT = 2
	};

	// méthodes InGame
	void InGameOnEvent(sf::Key::Code key);
	void InGameShow();
	
	// méthodes Inventory
	void InventoryOnEvent(sf::Key::Code key);
	void InventoryShow();
	
	Zone* zones_[GAME_HEIGHT][GAME_WIDTH];
	sf::Vector2i cds_zone_; // coordonnées de la zone courante
	
	Zone* active_zone_;
	Zone* next_zone_;
	Zone cave_;
	Player* player_;
	
	ControlPanel& panel_;
	
	// pointeur de la méthode de gestion des évènements
	void (Game::*on_event_meth_)(sf::Key::Code code);
	// pointeur de la méthode d'affichage'
	void (Game::*show_meth_)();
	
	sf::RenderWindow app_;
	
#ifdef FULLSCREEN_HACK
	sf::VideoMode DesktopMode;
#endif
};

#endif /* guard GAME_HPP */

