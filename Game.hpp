#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "ZoneContainer.hpp"
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
	
	void ChangeZone(ZoneContainer::Direction direction);
	void ChangeZoneContainer(ZoneContainer::MapName zone_name);
	
	inline Player* GetPlayer() const
	{
		return player_;
	}
	
	inline Zone* GetZone()
	{
		return zone_container_.GetActiveZone();
	}
	
	/**
	 * Téléporter le joueur
	 * @param[in] zone_name: zone container cible
	 * @param[in] zone_cds: coordonnées de la zone dans le conteneur
	 */
	void Teleport(ZoneContainer::MapName zone_name, const sf::Vector2i& zone_cds,
		const sf::Vector2i& tile_cds);
	
private:
	Game();
	Game(const Game&);
	~Game();

#ifdef DUMB_MUSIC
	void SetMusic(int value);
#endif
	
	// méthodes InGame
	void InGameOnEvent(sf::Key::Code key);
	void InGameShow();
	
	// méthodes Inventory
	void InventoryOnEvent(sf::Key::Code key);
	void InventoryShow();
	
	void DefaultUpdate(float frametime);

	enum Mode
	{
		IN_GAME, INVENTORY
	};
	
	void SetMode(Mode mode);
	
	// pointeur de la méthode de gestion des évènements
	void (Game::*on_event_meth_)(sf::Key::Code code);
	// pointeur de la méthode de mise à jour
	void (Game::*update_meth_)(float frametime);
	// pointeur de la méthode d'affichage'
	void (Game::*render_meth_)();

	// un seul conteneur de zones est chargé à la fois
	ZoneContainer zone_container_;
	// nom du prochain conteneur à charger
	ZoneContainer::MapName next_zone_name_;
	// coordonnées de la zone à activer si changement de conteneur
	sf::Vector2i next_zone_cds_;
	
	Player* player_;
	ControlPanel& panel_;
	
	sf::RenderWindow app_;
};

#endif /* GAME_HPP */

