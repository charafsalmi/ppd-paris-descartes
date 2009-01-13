#include <iostream>
#include <cassert>

#include "Tileset.hpp"
#include "MediaManager.hpp"
#include "tinyxml/tinyxml.h"

#define TILES_DEFINITION "data/xml/tiles.xml"


Tileset::Tileset()
{
	TiXmlDocument doc;
	// chargement des propriétés
	if (!doc.LoadFile(TILES_DEFINITION))
	{
		std::cerr << " [Tileset] erreur lors du chargement de "
			<< TILES_DEFINITION << " (" << doc.ErrorDesc() << ")" << std::endl;
		abort();
	}
	
	TiXmlHandle handle(&doc);
	TiXmlElement *elem = handle.FirstChildElement().FirstChildElement().Element();
	
	if (elem == NULL)
	{
		std::cerr << " [Tileset] impossible d'atteindre le noeud"
			<< std::endl;
		abort();
	}
	
	int tile_id;
	while (elem != NULL)
	{
		if (elem->QueryIntAttribute("id", &tile_id) == TIXML_SUCCESS)
		{
			// parsing des propriétés de la tile
			Tile::Effect effect = Tile::DEFAULT;
			int flag;
			if (elem->QueryIntAttribute("block", &flag) == TIXML_SUCCESS)
			{
				effect = flag ? Tile::BLOCK : Tile::DEFAULT;
			}
			else if (elem->QueryIntAttribute("hole", &flag) == TIXML_SUCCESS)
			{
				effect = flag ? Tile::HOLE : Tile::DEFAULT;
			}
			
			// ajout de la nouvelle tile spéciale
			specials_[tile_id] = effect;
#ifdef DEBUG
			printf("tile spéciale %d indexée\n", tile_id);
#endif
		}
		else
		{
			std::cerr << " [Tileset] tile id manquant" << std::endl;
		}
		
		elem = elem->NextSiblingElement();
	}
}


Tileset& Tileset::GetInstance()
{
	static Tileset self;
	return self;
}


void Tileset::MakeSprite(int tile_id, sf::Sprite& sprite)
{
	assert(tile_id < COUNT);
	sprite.SetImage(GET_IMG("tileset"));
	
	// on calcule le subrect de la tile grâce à son id
	// (la tile haut-gauche du tileset a l'id 0)
	sf::IntRect rect;
	rect.Left = (tile_id % WIDTH) * Tile::SIZE;
	rect.Right = rect.Left + Tile::SIZE;
	rect.Top = (tile_id / WIDTH) * Tile::SIZE;
	rect.Bottom = rect.Top + Tile::SIZE;
	sprite.SetSubRect(rect);
}


Tile::Effect Tileset::GetEffect(int tile_id) const
{
	TileIndexer::const_iterator it = specials_.find(tile_id);
	// la tile est-elle spéciale ?
	if (it != specials_.end())
	{
		return it->second;
	}
	// comportement d'une tile normale
	return Tile::DEFAULT;
}


bool Tileset::IsWalkable(int tile_id) const
{
	return GetEffect(tile_id) != Tile::BLOCK;
}

