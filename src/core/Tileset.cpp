#include <iostream>
#include <cassert>

#include "Tileset.hpp"
#include "../misc/MediaManager.hpp"
#include "../xml/tinyxml.h"
#include "../misc/Log.hpp"

#define TILES_DEFINITION "data/xml/tiles.xml"
#define ANIMATED_DELAY    0.25f


Tileset& Tileset::GetInstance()
{
	static Tileset self;
	return self;
}


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
	handle = handle.FirstChildElement();
	TiXmlElement* elem = handle.FirstChildElement("definitions").FirstChildElement().Element();

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
			else if (elem->QueryIntAttribute("teleport", &flag) == TIXML_SUCCESS)
			{
				effect = flag ? Tile::TELEPORT : Tile::DEFAULT;
			}

			// ajout de la nouvelle tile spéciale
			specials_[tile_id] = effect;
		}
		else
		{
		    OutputE << TILE_S << "Tile id manquant" << lEnd;
		}

		elem = elem->NextSiblingElement();
	}

	// tiles animées
	int from_id, to_id;
	elem = handle.FirstChildElement("animated").FirstChildElement().Element();
	while (elem != NULL)
	{
		bool ok = true;
		ok &= (elem->QueryIntAttribute("from", &from_id) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("to", &to_id) == TIXML_SUCCESS);
		if (ok)
		{
			animated_[from_id] = to_id;
			OutputD << TILE_S << from_id << " est animee" << lEnd;
		}
		else
		{
		    OutputW << TILE_S << "Animated invalide (ignore)" << lEnd;
		}
		elem = elem->NextSiblingElement();
	}

	timer_ = 0.f;
}


void Tileset::MakeSprite(int tile_id, sf::Sprite& sprite)
{
	assert(tile_id < COUNT);
	sprite.SetImage(GET_IMG("tileset"));

	// on calcule le subrect de la tile grâce à son id
	// (la tile première tile haut-gauche du tileset a l'id 0)
	sf::IntRect rect;
	rect.Left = (tile_id % WIDTH) * Tile::SIZE;
	rect.Right = rect.Left + Tile::SIZE;
	rect.Top = (tile_id / WIDTH) * Tile::SIZE;
	rect.Bottom = rect.Top + Tile::SIZE;
	sprite.SetSubRect(rect);
}


void Tileset::MakeAnimatedTile(int tile_id, AnimatedTile& tile)
{
	MakeSprite(tile_id, tile.sprite);
	tile.id = tile.frame = tile_id;
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


bool Tileset::NeedUpdate(float frametime) const
{
	if (timer_ >= ANIMATED_DELAY)
	{
		timer_ = 0;
		return true;
	}
	timer_ += frametime;
	return false;
}


void Tileset::UpdateAnimated(AnimatedTile& tile) const
{
	int last = animated_[tile.id];
	if (last == tile.frame)
	{
		tile.frame = tile.id;
	}
	else
	{
		++tile.frame;
	}
	sf::IntRect rect;
	rect.Left = (tile.frame % WIDTH) * Tile::SIZE;
	rect.Right = rect.Left + Tile::SIZE;
	rect.Top = (tile.frame / WIDTH) * Tile::SIZE;
	rect.Bottom = rect.Top + Tile::SIZE;
	tile.sprite.SetSubRect(rect);
}


bool Tileset::IsAnimated(int tile_id) const
{
	AnimatedIndexer::const_iterator it = animated_.find(tile_id);
	return it != animated_.end();
}
