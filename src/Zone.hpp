#ifndef ZONE_HPP
#define ZONE_HPP

#include <list>
#include <map>

#include "Entity.hpp"
#include "Item.hpp"
#include "Tileset.hpp"
#include "tinyxml/tinyxml.h"

#ifdef DUMB_MUSIC
#include "Music.hpp"
#endif

/**
 * Une zone de jeu, de la taille de l'écran
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
	void Load(const TiXmlHandle& handle);
	
	/**
	 * Détermine si la zone est chargée
	 * @return true si la zone est chargée, sinon false
	 */
	bool IsLoaded() const;
	
	/**
	 * Mettre à jour la zone
	 * @param[in] frametime: temps de la frame courante
	 */
	void Update(float frametime);
	
	/**
	 * Afficher la zone de jeu
	 * @param[in, out] app: fenêtre de rendu
	 */
	void Show(sf::RenderWindow& app) const;
	
	/**
	 * Ajouter une entité dans la zone de jeu
	 * @param[in, out] entity: entité à ajouter
	 */
	void AddEntity(Entity* entity);
	
	/**
	 * Retirer une entité de la zone de jeu
	 * @param[in] entity: entité à retirer
	 */
	void RemoveEntity(Entity* entity);
	
	/**
	 * Détermine si un mouvement est possible
	 * @param[in] emitter: entité qui cherche à se déplacer
	 * @param[in] rect: rectangle de la position issue du mouvement à tester
	 * @return true si le mouvement est possible, sinon false
	 */
	bool CanMove(Entity* emitter, const sf::FloatRect& rect) const;
	
	/**
	 * Ajouter un objet
	 * @param[in] id: type de l'objet
	 * @param[in] x: position x en pixels
	 * @param[in] y: position y en pixels
	 */
	void AddItem(char id, int x, int y);
	
	/**
	 * Obtenir le type d'une tile
	 * @param[in] x: position x en tiles
	 * @param[in] y: position y en tiles
	 */
	inline int GetTileAt(int x, int y) const
	{
		return tiles_[y][x];
	}
	
	/**
	 * Un téléporteur permet d'aller à un point donné, dans une zone donnée,
	 * dans un conteneur de zone donné
	 */
	struct Teleporter
	{
		int zone_container; // conteneur cible
		sf::Vector2i zone_coords; // position de la zone cible (en tiles)
		sf::Vector2i tile_coords; // position cible dans la zone (en pixels)
	};
	
	/**
	 * Obtenir un téléporteur placé sur la zone
	 * @param[in] x: position x en tiles
	 * @param[in] y: position y en tiles
	 * @param[out] tp: téléporteur à récupérer
	 * @return si téléporteur trouvé, sinon false
	 */
	bool GetTeleport(int x, int y, Teleporter& tp) const;
	
	/**
	 * Obtenir l'image des tiles en arrière-plan
	 * @return image de toutes les tiles de la zone
	 */
	const sf::Image* GetBackground() const;
	
private:
	/**
	 * Ajouter une entité dans la zone de jeu
	 * @param[in] name: identifiant
	 * @param[in] x: tile x
	 * @param[in] y: tile y
	 */
	//void AddEntity(const char* name, int x, int y);
	
	/**
	 * Désallouer toutes les entités et tous les items de la zone
	 */
	void Purge();
	
	// listes des entités
	typedef std::list<Entity*> EntityList;
	mutable EntityList entities_;
	
	// listes des items
	typedef std::list<Item*> ItemList;
	ItemList items_;
	
	// dictionnaire des téléporteurs
	typedef std::map<int, Teleporter> TeleportIndexer;
	TeleportIndexer teleporters_;
	
	bool loaded_;
	int tiles_[HEIGHT][WIDTH];
	// indique là on l'on peut marcher (pour ne pas le recalculer)
	bool walkable_[HEIGHT][WIDTH];
	sf::Image tiles_img_; // image des tiles de la zone
	sf::Sprite tiles_sprite_; // sprite associé aux tiles
#ifdef DUMB_MUSIC
	int music_index_;
#endif
};

#endif /* ZONE_HPP */

