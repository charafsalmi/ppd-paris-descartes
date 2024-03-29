#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "ZoneContainer.hpp"
#include "InputController.hpp"
#include "../gui/ControlPanel.hpp"
#include "../gui/WinPause.hpp"
#include "../gui/MainMenu.hpp"
#include "../gui/Option.hpp"
#include "../misc/StringUtils.hpp"
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
	void ChangeMap(const std::string& map_name);

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
		return map_.GetActiveZone();
	}

	inline float GetElapsedTime() const
	{
		return clock_.GetElapsedTime();
	}

private:
	Game();
	Game(const Game&);
	~Game();

#define SAVE_FILE   "config/save.cfg"

	/**
	 * Charger un fichier de configuration
	 * @param[in] str: nom du fichier de configuration
	 */
	bool LoadConfig(const std::string & str);

    /// Sauvegarde un fichier de configuration système
	/// @param[in] str: Nom du fichier de configuration
	void SaveConfig(const std::string & str) const;

	/**
	 * Enregistrer la progression du joueur
	 * @param[in] filename: fichier de sauvegarde
	 */
	void SaveProgression(const char* filename) const;

	/**
	 * Charger la progression du joueur
	 * @param[in] filename: fichier de sauvegarde
	 */
	bool LoadProgression(const char* filename);

	// Prend une capture d'écran de la fenêtre
	void TakeScreenshot(const char* directory);


	// les différents états possibles du jeu
	enum Mode
	{
		MAIN_MENU, IN_GAME, GAME_OVER, INVENTORY, PAUSE, MINI_MAP, OPTION
	};

    /**
     * Passe le jeu dans un mode différent
     * @param mode: mode de jeu dans lequel passer
     */
	void SetMode(Mode mode);

	/*
	callbacks du mode de jeu en cours
	__OnEvent: méthode de la gestion des évènements
	__Update: méthode de mise à jour de la scène
	__Show: méthode de rendu (affichage)
	*/

	// mode InGame
	void InGameOnEvent(const sf::Event& event, input::Action action);
	void InGameShow();

	// mode Inventory
	void InventoryOnEvent(const sf::Event& event, input::Action action);
	void InventoryShow();

	// mode Inventory
	void MainMenuOnEvent(const sf::Event& event, input::Action action);
	void MainMenuShow();

    // mode Pause
	void PauseOnEvent(const sf::Event& event, input::Action action);
	void PauseShow();

    // mode Option
	void OptionOnEvent(const sf::Event& event, input::Action action);
	void OptionShow();

	// mode GameOver
	void GameOverOnEvent(const sf::Event& event, input::Action action);
	void GameOverShow();

	// mode MiniMap
	void MiniMapShow();

	// Mise à jour générique
	void DefaultUpdate(float frametime);
	void NoUpdate(float);

	// pointeur de la méthode de gestion des évènements
	void (Game::*on_event_meth_)(const sf::Event& event, input::Action action);
	// pointeur de la méthode de mise à jour
	void (Game::*update_meth_)(float frametime);
	// pointeur de la méthode d'affichage'
	void (Game::*render_meth_)();

	struct Options
	{
		// Settings
		bool panel_on_top;

		// Engine
		int fps;
		int verbosity;
	};
	Options options_;

	bool running_;
	// un seul conteneur de zones est chargé à la fois
	ZoneContainer map_;
	// nom de la prochaine carte à charger
	std::string map_name_;
	bool need_map_update_;

	// coordonnées de la zone à activer si changement de conteneur
	sf::Vector2i next_zone_cds_;

	Mode mode_;

    MainMenu mmenu_;
    WinPause pause_;
    Option *option_win_;
	Player* player_;
	ControlPanel& panel_;
	BitmapString message_;

	sf::Clock clock_;

#ifdef CONSOLE_TEST
    LogConsole *log_;
#endif
	MiniMap* mini_map_;
	sf::RenderWindow app_;
	InputController& controller_;
};

#endif // GAME_HPP

