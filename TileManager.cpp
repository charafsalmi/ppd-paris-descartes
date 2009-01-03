#include <iostream>

#include "TileManager.hpp"

#define TILES_DEFINITION "data/xml/tiles.xml"


TileManager::TileManager()
{
	// chargement des propriétés
	if (!doc_.LoadFile(TILES_DEFINITION))
	{
		std::cerr << "[TileManager] erreur lors du chargement de "
			<< TILES_DEFINITION << " (" << doc_.ErrorDesc() << ")" << std::endl;
		abort();
	}
	
	TiXmlHandle handle(&doc_);
	TiXmlElement *elem = handle.FirstChildElement().FirstChildElement().Element();
	
	if (elem == NULL)
	{
		std::cerr << "[TileManager] impossible d'atteindre le noeud"
			<< std::endl;
		abort();
	}
	
	int tile_id;
	while (elem != NULL)
	{
		if (elem->QueryIntAttribute("id", &tile_id) == TIXML_SUCCESS)
		{
			// parsing des propriétés de la tile
			TileProperties props;
			if (elem->QueryIntAttribute("walkable", &props.walkable) != TIXML_SUCCESS)
			{
				props.walkable = 1; // walkable par défaut
			}
			
			// ajout de la nouvelle tile spéciale
			specials_[tile_id] = props;
#ifdef DEBUG
			printf("tile spéciale %d indexée\n", tile_id);
#endif
		}
		else
		{
			std::cerr << "[TileManager] tile id manquant" << std::endl;
		}
		
		elem = elem->NextSiblingElement();
	}
}


TileManager& TileManager::GetInstance()
{
	static TileManager self;
	return self;
}


bool TileManager::IsWalkable(int tile_id) const
{
	TileIndexer::const_iterator it = specials_.find(tile_id);
	// la tile est-elle spéciale ?
	if (it != specials_.end())
	{
		return it->second.walkable;
	}
	// comportement d'une tile normale
	return true;
}

