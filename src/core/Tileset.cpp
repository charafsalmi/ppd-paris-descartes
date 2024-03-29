#include <cassert>
#include <sstream>

#include "Tileset.hpp"
#include "../misc/MediaManager.hpp"
#include "../misc/Die.hpp"
#include "../xml/tinyxml.h"

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
		DIE("can't load tiles definition '%s' (tinyxml: %s)", TILES_DEFINITION, doc.ErrorDesc());
	}

	TiXmlHandle handle(&doc);
	handle = handle.FirstChildElement();
	TiXmlNode* node = handle.FirstChildElement("definitions").FirstChildElement().Node();

	while (node != NULL)
	{
		TiXmlElement* elem = node->ToElement();
		std::string effect_name = elem->Attribute("effect");
		Tile::Effect effect = Tile::DEFAULT;
		if (effect_name == "hole")
		{
			effect = Tile::HOLE;
		}
		else if (effect_name == "water")
		{
			effect = Tile::WATER;
		}
		else if (effect_name == "block")
		{
			effect = Tile::BLOCK;
		}
		else if (effect_name == "teleport")
		{
			effect = Tile::TELEPORT;
		}

		std::istringstream tiles(elem->GetText());
		int tile_id;
		while (tiles.good())
		{
			tiles >> tile_id;
			specials_[tile_id] = effect;

		}
		node = node->NextSibling();
	}

	// tiles animées
	int from_id, to_id;
	TiXmlElement* elem = handle.FirstChildElement("animated").FirstChildElement().Element();
	while (elem != NULL)
	{
		bool ok = true;
		ok &= (elem->QueryIntAttribute("from", &from_id) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("to", &to_id) == TIXML_SUCCESS);
		if (ok)
		{
			animated_[from_id] = to_id;
		}
		else
		{
			puts("warning: invalid animated tiles (ignored)");
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

