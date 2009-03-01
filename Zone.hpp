#ifndef ZONE_HPP
#define ZONE_HPP

#include <list>

#include "Entity.hpp"
#include "Item.hpp"
#include "Tileset.hpp"
#include "tinyxml/tinyxml.h"

#ifdef DUMB_MUSIC
#include "Music.hpp"
#endif

/**
 * Une zone de jeu
 */
class Zone
{
public:
	enum
	{
		// Dimensions de la zone en nombre de tiles
		WIDTH = 20, HEIGHT = 16
	};
	
	Zone();
	
	~Zone();
	
	/**
	 * Charger le contenu de la zone
	 * @param[in] handle: nœud XML décrivant la zone
	 */
	void Load(const TiXmlHandle& handle, sf::RenderWindow& app);
	
	/**
	 * Mettre à jour la zone
	 * @param[in] frametime: temps de la frame courante
	 */
	void Update(float frametime);
	
	/**
	 * Afficher la zone de jeu
	 * @param[in] app: fenêtre de rendu
	 */
	void Show(sf::RenderWindow& app) const;
	
	/**
	 * Ajouter une entité dans la zone de jeu
	 * @param[in, out] entity: entité ajoutée
	 */
	void AddEntity(Entity* entity);
	
	/**
	 * Retirer une entité de la zone de jeu
	 * @param[in] entity: entité à enlever
	 */
	void RemoveEntity(Entity* entity);
	
	/**
	 * Détermine si un mouvement est possible
	 * @param[in] emitter: entité qui cherche à se déplacer
	 * @param[in] rect: rectangle de la position issue du mouvement à tester
	 * @return true si le mouvement est possible, sinon false
	 */
	bool CanMove(Entity* emitter, const sf::FloatRect& rect);
	
	/**
	 * Ajouter un objet
	 */
	void AddItem(char id, int x, int y);

	inline int GetTileAt(int x, int y)
	{
		return tiles_[y][x];
	}
	

	/**
	 * Un téléporteur permet d'aller à un point donné, dans une zone donnée,
	 * dans un conteneur de zone donné
	 */
	struct Teleporter
	{
		int zone_container;
		sf::Vector2i zone_coords;
		sf::Vector2i tile_coords;
	};
	
	bool GetTeleport(int x, int y, Teleporter& tp);
	
	/**
	 * Obtenir l'image des tiles du décor
	 */
	const sf::Image* GetBackground() const;
	

	
private:
	
	
	
	/**
	 * Ajouter une entité dans la zone de jeu
	 * @param[in] name: identifiant
	 * @param[in] x: tile x
	 * @param[in] y: tile y
	 */
	void AddEntity(const char* name, int x, int y);
	
	/**
	 * Désallouer toutes les entités et tous les items de la zone
	 */
	void Purge();
	
	typedef std::list<Entity*> EntityList;
	typedef std::list<Item*> ItemList;
	
	mutable EntityList entities_;
	ItemList items_;
	
	std::map<int, Teleporter> teleporters_;
	
	int tiles_[HEIGHT][WIDTH];
	bool walkable_[HEIGHT][WIDTH];
	sf::Image tiles_img_; // image des tiles de la zone
	sf::Sprite tiles_sprite_; // sprite associé aux tiles
#ifdef DUMB_MUSIC
	int zone_music_index_;
#endif
};

#endif /* ZONE_HPP */

