#ifndef TILESET_HPP
#define TILESET_HPP

#include <map>
#include <SFML/Graphics.hpp>


namespace Tile
{
	enum
	{
		SIZE = 32 // taille en pixels
	};
	enum Effect
	{
		DEFAULT, // aucun effet
		BLOCK,
		WATER,
		HOLE,
		TELEPORT
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
		WIDTH = 16, // nombre de tiles en largeur
		HEIGHT = 20, // en hauteur
		COUNT = WIDTH * HEIGHT // nombre total
	};
	
	/**
	 * @return l'instance unique
	 */
	static Tileset& GetInstance();
	
	/**
	 * Fabriquer un sprite à partir d'un id
	 * @param[in] tile_id: type de tile
	 * @param[out] sprite: sprite à fabriquer
	 */
	static void MakeSprite(int tile_id, sf::Sprite& sprite);
	
	/**
	 * Obtenir l'effet d'un type de tile
	 * @param[in] tile_id: type de tile
	 * @return effet associé
	 */
	Tile::Effect GetEffect(int tile_id) const;
	
	/**
	 * Déterminer si l'on peut marcher sur un type tile
	 * @param[in] tile_id: type de tile
	 * @return true si l'on peut marcher dessus, sinon false
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

