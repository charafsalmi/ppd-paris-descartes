#include <cassert>

#include "Tile.hpp"
#include "MediaManager.hpp"


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
	rect.Bottom = rect.Right + SIZE;
	SetSubRect(rect);
	
	SetPosition(pos);
	
	/* TODO: comment savoir si la tile est walkable ?
	solution 1 : on dit que toutes les tiles d'id supérieur à nombre_magique
	sont des tiles walkables, et les autres ne le sont pas...
	(il suffit de diviser le tileset en deux)
	inconvénient : il faut mettre à jour ce nombre magique quand on modifie
	le tileset
	
	solution 2 : on fait deux tilesets, un pour les tiles walkables, et un
	autre pour les tiles non-walkables.
	inconvénient : il faut passer le nom du tileset en paramètre pour
	construire la tile
	
	De plus, si on associe un effet d'environnement à la tile (le sable ralentit,
	on peut nager ou se noyer dans l'eau, tomber dans des trous, etc.) il faudra
	passer ces paramètres supplémentaires.
	-> Besoin de définir un format de représentation des tiles sur fichier
	*/
	walkable_ = id != 1; // seule la tile 1 (eau) n'est pas walkable (juste pour tester)
}

