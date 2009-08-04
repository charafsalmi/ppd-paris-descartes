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
		DEFAULT  = 1 << 0, // aucun effet
		BLOCK    = 1 << 1,
		WATER    = 1 << 2,
		HOLE     = 1 << 3,
		TELEPORT = 1 << 4
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

	struct AnimatedTile
	{
		sf::Sprite sprite;
		int frame;
		int id;
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
	 * Fabriquer une tile animée
	 * @param[in] tile_id: type de tile
	 * @param[out] tile: tile animée à initialiser
	 */
	static void MakeAnimatedTile(int tile_id, AnimatedTile& tile);

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

	/**
	 * Détermine si les tiles animées doivent être mises à jour
	 * @return true si mise à jour nécessaire, sinon false
	 */
	bool NeedUpdate(float frametime) const;

	/**
	 * Mettre à jour une tile animée
	 * @param[in, out] tile
	 */
	void UpdateAnimated(AnimatedTile& tile) const;

	/**
	 * Détermine si une tile est animée
	 * @param[in] tile_id: type de tile
	 */
	bool IsAnimated(int tile_id) const;

private:
	Tileset();
	Tileset(const Tileset& other);
	Tileset& operator=(const Tileset& other);

	typedef std::map<int, Tile::Effect> TileIndexer;
	TileIndexer specials_;

	typedef std::map<int, int> AnimatedIndexer;
	mutable AnimatedIndexer animated_;

	mutable float timer_;
};

#endif // TILESET_HPP

