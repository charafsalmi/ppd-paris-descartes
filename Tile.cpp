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
	
	/* TODO:
	il faut charger au préalable un descripteur des tiles (TileManager ?)
	qui va attribuer les paramètres pour chaque id de tile.
	
	il ne faut pas stocker les paramètres pour chaque tile, mais pour
	chaque type de tile
	une tile doit stocker uniquement son id, et demander au descripteur
	les caractéristiques de cet id :
	bool Tile::IsWalkable() const
		return descripteur.IsWalkable(id_);
	
	on peut aussi associer un effet d'environnement à la tile (le sable ralentit,
	on peut nager ou se noyer dans l'eau, tomber dans des trous, etc.) il faudra
	passer ces paramètres supplémentaires au descripteur
	*/
	// en attendant, valeurs magiques de tiles non walkables (eau et poteaux)
	walkable_ = manager_.IsWalkable(id);
	
	//walkable_ = id != 13 && id != 14 && id != 6; 
}

