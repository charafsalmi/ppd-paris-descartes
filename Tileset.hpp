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
		WATER, 
		HOLE
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
		WIDTH = 16, // nombre de tile en larger
		HEIGHT = 20, // en hauteur
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
	
	Tile::Effect GetEffect(const int tile_id) const
	{
		ConstTileIter it = specials_.find(tile_id);
		if (it != specials_.end())
		{
			return (*it).second;
		}
		return Tile::DEFAULT;
	}

	typedef std::map<int, int> EffectArgs;
	typedef EffectArgs::iterator EffectIter;	
private:
	Tileset();
	Tileset(const Tileset& other);
	Tileset& operator=(const Tileset& other);
	
	typedef std::map<int, Tile::Effect> TileIndexer;
	typedef TileIndexer::iterator TileIter;
	typedef TileIndexer::const_iterator ConstTileIter;
	TileIndexer specials_;

};


#endif /* TILESET_HPP */

