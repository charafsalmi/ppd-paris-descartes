#include <cassert>

#include "Tile.hpp"
#include "MediaManager.hpp"


TileManager& Tile::manager_ = TileManager::GetInstance();


void Tile::Build(int id, const sf::Vector2f& pos)
{
	static MediaManager& media = MediaManager::GetInstance();
	assert(id < SHEET_COUNT);
	
	SetImage(media.GetImage("tileset"));
	// on calcule le subrect de la tile grâce à son id
	// (la tile haut-gauche du tileset a l'id 0)
	sf::IntRect rect;
	rect.Left = (id % SHEET_WIDTH) * SIZE;
	rect.Right = rect.Left + SIZE;
	rect.Top = (id / SHEET_WIDTH) * SIZE;
	rect.Bottom = rect.Top + SIZE;
	SetSubRect(rect);
	
	SetPosition(pos);
	
	walkable_ = manager_.IsWalkable(id);
}

