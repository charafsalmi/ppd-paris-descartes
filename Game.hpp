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
	
	void Teleport(const char* zone);
	
	inline Player* GetPlayer() const
	{
		return player_;
	}
	
	inline Zone* GetZone()
	{
		return active_zone_;
	}

private:
	Game();
	Game(const Game&);
	~Game();

#ifdef DUMB_MUSIC
	void SetMusic(int value);
#endif

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
	
	// méthodes Scrolling
	void ScrollingOnEvent(sf::Key::Code key);
	void ScrollingUpdate(float frametime);
	void ScrollingShow();
	
	Zone* zones_[GAME_HEIGHT][GAME_WIDTH];
	sf::Vector2i cds_zone_; // coordonnées de la zone courante
	
	Zone* active_zone_;
	Zone* next_zone_;
	Zone cave_;
	Player* player_;
	
	ControlPanel& panel_;
	
	// pointeur de la méthode de gestion des évènements
	void (Game::*on_event_meth_)(sf::Key::Code code);
	// pointeur de la méthode de mise à jour
	void (Game::*update_meth_)(float frametime);
	// pointeur de la méthode d'affichage'
	void (Game::*show_meth_)();
	
	struct ScrollZone
	{
		sf::Sprite current;
		sf::Sprite next;
		Direction dir;
		float timer;
		bool need_scrolling;
	};
	ScrollZone scroll_;
	
	sf::RenderWindow app_;
};

#endif /* GAME_HPP */

