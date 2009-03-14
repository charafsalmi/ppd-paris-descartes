#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "ZoneContainer.hpp"
#include "../gui/ControlPanel.hpp"
#include "../gui/MyWin.hpp"
#include "../gui/WinPause.hpp"
#include "../misc/Misc.hpp"
#include "../misc/BitmapString.hpp"
#include "../misc/Log.hpp"

#ifdef WINDOW_TEST
#include "../misc/LogConsole.hpp"
#endif

class MiniMap;
class Player;

class Game
{
public:
	static Game& GetInstance();

	/**
	 * Initialiser le jeu
	 */
	void Init();

	/**
	 * Lancer l'application
	 */
	int Run();

	/**
	 * Changer la zone courante
	 */
	void ChangeZone(ZoneContainer::Direction direction);

	/**
	 * Changer la carte courante
	 */
	void ChangeZoneContainer(ZoneContainer::MapName map_name);

	/**
	 * Téléporter le joueur
	 * @param[in] teleporter: cible de la téléportation
	 */
	void Teleport(const Zone::Teleporter& tp);

	/**
	 * Stopper le déroulement de la partie
	 */
	void EndGame();

	inline Player* GetPlayer() const
	{
		return player_;
	}

	inline sf::RenderWindow& GetApp()
	{
		return app_;
	}

	inline Zone* GetZone()
	{
		return zone_container_.GetActiveZone();
	}

	inline float GetElapsedTime() const
	{
		return clock_.GetElapsedTime();
	}

private:
	Game();
	Game(const Game&);
	~Game();

	// Prend une capture d'écran de la fenêtre
	void TakeScreenshot(const char* directory);

#ifdef DUMB_MUSIC
	void SetMusic(int value);
#endif

	// callbacks

	// méthodes InGame
	void InGameOnEvent(const sf::Event& event);
	void InGameShow();

	// méthodes Inventory
	void InventoryOnEvent(const sf::Event& event);
	void InventoryShow();

	// méthode menu principal
	void MainMenuOnEvent(const sf::Event& event);
	void MainMenuUpdate(float frametime);
	void MainMenuShow();

    // méthodes Pause
	void PauseOnEvent(const sf::Event& event);
	void PauseUpdate(float frametime);
	void PauseShow();

	// méthodes GameOver
	void GameOverOnEvent(const sf::Event& event);
	void GameOverUpdate(float frametime);
	void GameOverShow();

	// méthodes MiniMap
	void MiniMapOnEvent(const sf::Event& event);
	void MiniMapShow();

	// update générique
	void DefaultUpdate(float frametime);

	enum Mode
	{
		IN_GAME, GAME_OVER, INVENTORY, PAUSE, MINI_MAP
	};

    //Mode mode_;

	void SetMode(Mode mode);

	// pointeur de la méthode de gestion des évènements
	void (Game::*on_event_meth_)(const sf::Event& event);
	// pointeur de la méthode de mise à jour
	void (Game::*update_meth_)(float frametime);
	// pointeur de la méthode d'affichage'
	void (Game::*render_meth_)();

	struct Options
	{
		bool panel_on_top;
		int verbosity;
	};
	Options options_;

	bool running_;
	// un seul conteneur de zones est chargé à la fois
	ZoneContainer zone_container_;
	// nom du prochain conteneur à charger
	ZoneContainer::MapName next_map_name_;
	// coordonnées de la zone à activer si changement de conteneur
	sf::Vector2i next_zone_cds_;

    WinPause pause_;
	Player* player_;
	ControlPanel& panel_;
	BitmapString message_;

	sf::Clock clock_;

#ifdef CONSOLE_TEST
    LogConsole *log_;
#endif
#ifdef WINDOW_TEST
	MyWin fen_;
#endif
	MiniMap* mini_map_;
	sf::RenderWindow app_;
};

#endif /* GAME_HPP */

