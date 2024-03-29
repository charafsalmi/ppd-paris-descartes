#ifndef ZONE_HPP
#define ZONE_HPP

#include <list>
#include <map>
#include <SFML/Graphics.hpp>

#include "Tileset.hpp"
#include "../entities/Item.hpp"
#include "../xml/tinyxml.h"

class Entity;
class Decor;

/**
 * Une zone de jeu, de la taille de l'écran
 */
class Zone: public sf::Drawable
{
public:
	enum
	{
		// Dimensions de la zone en nombre de tiles
		WIDTH = 20, HEIGHT = 16,
		// Dimensions en pixels
		WIDTH_PX = WIDTH * Tile::SIZE, HEIGHT_PX = HEIGHT * Tile::SIZE
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
	 * Supprimer tous les coups de la zone
	 */
	void ClearHits();

	/**
	 * Détermine si un mouvement est possible
	 * @param[in] rect: rectangle de la position issue du mouvement à tester
	 * @param[in] tileflag: masque de bits des types de tiles pouvant être franchis
	 * @return true si le mouvement est possible, sinon false
	 */
	bool CanMove(const sf::FloatRect& rect, int tileflag = Tile::DEFAULT) const;

	/**
	 * Ajouter un objet
	 * @param[in] type: type de l'objet
	 * @param[in] x: position x en pixels
	 * @param[in] y: position y en pixels
	 */
	void AddItem(Item::Type type, int x, int y);

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
	 * Obtenir le nom de la musique correspondant à cette zone
	 * @return nom de la musique
	 */
	inline const std::string& GetMusicName() const
	{
		return music_name_;
	}

	/**
	 * Un téléporteur permet d'aller à un point donné, dans une zone donnée,
	 * dans un conteneur de zone donné
	 */
	struct Teleporter
	{
		std::string map_name; // carte cible
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

	void Interact(Entity* player);

private:
	// inherited
	void Render(sf::RenderTarget& target) const;

	/**
	 * Ajouter un décor dans la zone de jeu
	 * @param[in] decor: décor à ajouter
	 * @param[in] x: tile x
	 * @param[in] y: tile y
	 */
	void AddDecor(Entity* decor, int x, int y);

	/**
	 * Désallouer toutes les entités et tous les items de la zone
	 */
	void Purge();

	// liste des entités
	typedef std::list<Entity*> EntityList;
	mutable EntityList entities_;

	// liste des items
	typedef std::list<Item*> ItemList;
	ItemList items_;

	// dictionnaire des téléporteurs
	typedef std::map<int, Teleporter> TeleportIndexer;
	TeleportIndexer teleporters_;

	bool loaded_;
	int tiles_[HEIGHT][WIDTH];
	// indique là on l'on peut marcher (pour ne pas le recalculer)
	Tile::Effect walkable_[HEIGHT][WIDTH];
	sf::Image tiles_img_; // image des tiles de la zone
	sf::Sprite tiles_sprite_; // sprite associé aux tiles

	std::vector<Tileset::AnimatedTile> animated_;
	int last_id_;

	std::string music_name_;
};

#endif // ZONE_HPP

