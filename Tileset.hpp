#ifndef TILESET_HPP
#define TILESET_HPP

#include <map>
#include <SFML/Graphics.hpp>

namespace Tile
{
	enum
	{
		SIZE = 32
	};
	enum Effect
	{
		DEFAULT, // aucun effet
		BLOCK,
		WATER
	};
}

class Tileset
{
public:
	/**
	 * Indique le nombre de tiles contenues dans la feuille de tiles (tileset)
	 */
	enum
	{
		WIDTH = 8, // nombre de tile en larger
		HEIGHT = 8, // en hauteur
		COUNT = WIDTH * HEIGHT // nombre total
	};
	
	/**
	 * Récupérer l'instance unique
	 */
	static Tileset& GetInstance();
	
	/**
	 * Fabriquer un sprite à partir d'un id
	 * @param[in] tile_id: id de la tile
	 * @param[out] sprite: sprite à fabriquer
	 */
	static void MakeSprite(int tile_id, sf::Sprite& sprite);
	
	/**
	 * Déterminer si l'on peut marcher sur un type tile
	 */
	bool IsWalkable(int tile_id) const;
	
private:
	Tileset();
	Tileset(const Tileset& other);
	Tileset& operator=(const Tileset& other);
	
	typedef std::map<int, Tile::Effect> TileIndexer;
	
	TileIndexer specials_;
};


#endif /* TILESET_HPP */

