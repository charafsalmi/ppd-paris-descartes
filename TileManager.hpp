#ifndef TILEMANAGER_HPP
#define TILEMANAGER_HPP

#include <map>

#include "tinyxml/tinyxml.h"


class TileManager
{
public:
	/**
	 * Récupérer l'instance unique
	 */
	static TileManager& GetInstance();
	
	/**
	 * Détermine si un type de tile est "walkable"
	 */
	bool IsWalkable(int tile_id) const;
	
private:
	TileManager();
	TileManager(const TileManager& other);
	TileManager& operator=(const TileManager& other);
	
	/**
	 * Les propriétés d'une tile
	 */
	struct TileProperties
	{
		int walkable;
	};
	
	typedef std::map<int, TileProperties> TileIndexer;
	
	TileIndexer specials_;
	TiXmlDocument doc_;
};


#endif /* TILEMANAGER_HPP */

