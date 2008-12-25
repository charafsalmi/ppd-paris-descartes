#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>


class Tile: public sf::Sprite
{
public:
	/**
	 * Indique le nombre de tiles contenues dans la feuille de tiles (tileset)
	 */
	enum
	{
		SHEET_WIDTH = 7, // nombre de tiles en largeur
		SHEET_HEIGHT = 3, // en hauteur
		SHEET_COUNT = SHEET_WIDTH * SHEET_HEIGHT // nombre total
	};
	
	enum
	{
		SIZE = 24 // taille en pixels d'une tile
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
	bool walkable_;
};

#endif /* guard TILE_HPP */

