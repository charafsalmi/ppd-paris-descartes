#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>

#include "TileManager.hpp"


class Tile: public sf::Sprite
{
public:
	/**
	 * Indique le nombre de tiles contenues dans la feuille de tiles (tileset)
	 */
	enum
	{
		SHEET_WIDTH = 8, // nombre de tiles en largeur
		SHEET_HEIGHT = 8, // en hauteur
		SHEET_COUNT = SHEET_WIDTH * SHEET_HEIGHT, // nombre total
		
		SIZE = 32 // taille en pixels d'une tile
	};
	
	enum Effect
	{
		NONE, WATER, HOLE
	};
	
	/**
	 * Construire une tile
	 * @param[in] id: indice de la tile dans la tilesheet
	 * @param[in] pos: position de la tile dans la zone
	 */
	void Build(int id, const sf::Vector2f& pos);
	
	/**
	 * Détermine si l'on peut marcher sur la tile
	 * @return true si on peut marcher, sinon false
	 */
	inline bool Walkable() const
	{
		return walkable_;
	}
	
private:
	static TileManager& manager_;
	bool walkable_;
	//Effect effet_;
};

#endif /* guard TILE_HPP */

