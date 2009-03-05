#include <algorithm>
#include <iostream>
#include <sstream>
#include <cassert>
#include <cstring>

#include "Zone.hpp"
#include "StaticItem.hpp"
#include "MediaManager.hpp"
#include "UnitFactory.hpp"
#include "Game.hpp"
#include "ZoneContainer.hpp"


Zone::Zone()
{
	loaded_ = false;
}


Zone::~Zone()
{
	Purge();
}


void Zone::Load(const TiXmlHandle& handle)
{
	// chargement des tiles
	sf::RenderWindow& app = Game::GetInstance().GetApp();
	static const Tileset& tileset = Tileset::GetInstance();
	const TiXmlElement* elem = handle.FirstChildElement("tiles").Element();
	
	std::istringstream all_tiles(elem->GetText());
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			assert(!all_tiles.eof()); // TODO: lecture robuste
			
			int tile_id;
			all_tiles >> tile_id;
			walkable_[i][j] = tileset.IsWalkable(tile_id);
			tiles_[i][j] = tile_id;
			
			// création et dessin du sprite
			sf::Sprite tile;
			tileset.MakeSprite(tile_id, tile);
			sf::Vector2f pos(j * Tile::SIZE, i * Tile::SIZE);
			tile.SetPosition(pos);
			
			app.Draw(tile);
		}
	}
	
	// chargement des entités
	UnitFactory& factory = UnitFactory::GetInstance();
	elem = handle.FirstChildElement("entities").FirstChildElement().Element();
	while (elem != NULL)
	{
		bool ok = true;
		int x, y, id;
		ok &= (elem->QueryIntAttribute("id", &id) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("x", &x) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("y", &y) == TIXML_SUCCESS);
		if (ok)
		{
			Entity* entity = factory.Make(id, sf::Vector2f(x, y));
			if (entity != NULL)
			{
				AddEntity(entity);
			}
		}
		else
		{
			std::cerr << " [Zone] attributs invalides" << std::endl;
		}
		elem = elem->NextSiblingElement();
	}
	
	// chargement des téléporteurs
	elem = handle.FirstChildElement("teleporters").FirstChildElement().Element();
	while (elem != NULL)
	{
		bool ok = true;
		int x, y, zone_x, zone_y, tile_x, tile_y, zc_id;
		// coordonnées tile du téléporteur
		ok &= (elem->QueryIntAttribute("x", &x) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("y", &y) == TIXML_SUCCESS);
		// zone container cible
		ok &= (elem->QueryIntAttribute("container", &zc_id) == TIXML_SUCCESS);
		// coordonnées de la zone cible
		ok &= (elem->QueryIntAttribute("zone_x", &zone_x) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("zone_y", &zone_y) == TIXML_SUCCESS);
		// coordonées de la tile cible
		ok &= (elem->QueryIntAttribute("tile_x", &tile_x) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("tile_y", &tile_y) == TIXML_SUCCESS);
		
		if (!ok)
		{
			std::cerr << " [Zone] teleporteur invalide ignoré" << std::endl;
		}
		else
		{
			Teleporter tp;
			tp.zone_container = zc_id;
			tp.zone_coords = sf::Vector2i(zone_x, zone_y);
			tp.tile_coords = sf::Vector2i(tile_x, tile_y);
			int key = y * WIDTH + x;
			teleporters_[key] = tp;
		}
		elem = elem->NextSiblingElement();
	}
	
	// création de l'image des tiles
	tiles_img_ = app.Capture();
	tiles_sprite_.SetImage(tiles_img_);
	app.Clear();
	loaded_ = true;
}


bool Zone::IsLoaded() const
{
	return loaded_;
}


void Zone::Update(float frametime)
{
	// collisions avec les entités
	EntityList::iterator it;
	for (it = entities_.begin(); it != entities_.end(); ++it)
	{
		(**it).Update(frametime);
	}

	// collisions avec les items
	ItemList::iterator it2;
	Player* player = Game::GetInstance().GetPlayer();
	sf::FloatRect player_rect;
	player->GetFloorRect(player_rect);
	sf::FloatRect item_rect;
	for (it2 = items_.begin(); it2 != items_.end();)
	{
		if ((**it2).IsDead())
		{
#ifdef DEBUG
			printf(" [Zone] %s supprimé\n", (**it2).GetName().c_str());
#endif
			delete *it2;
			it2 = items_.erase(it2);
		}
		else
		{
			(**it2).GetRect(item_rect);
			if (item_rect.Intersects(player_rect))
			{
				(**it2).OnCollide(*player);
				break;
			}
			++it2;
		}
	}
}


void Zone::Show(sf::RenderWindow& app) const
{
	// affichage des tiles
	app.Draw(tiles_sprite_);
	
	// affichage des items
	ItemList::const_iterator it2;
	for (it2 = items_.begin(); it2 != items_.end(); ++it2)
	{
		app.Draw(**it2);
	}
	
	// affichage des entités
	entities_.sort(Entity::PtrComp);
	EntityList::const_iterator it;
	for (it = entities_.begin(); it != entities_.end(); ++it)
	{
		app.Draw(**it);
	}
}


bool Zone::CanMove(Entity* emitter, const sf::FloatRect& rect) const
{
	// si hors de la zone
	if (rect.Top < 0 || rect.Left < 0 || rect.Bottom > Tile::SIZE * HEIGHT
		|| rect.Right > Tile::SIZE * WIDTH)
	{
		return false;
	}
	// on regarde pour chaque coin du rectangle
	// si la tile en dessous est walkable
	int left = (int) rect.Left / Tile::SIZE;
	int top = (int) rect.Top / Tile::SIZE;
	int right = (int) rect.Right / Tile::SIZE;
	int bottom = (int) rect.Bottom / Tile::SIZE;
	
	// si collision avec un élément statique
	if (!walkable_[top][left] || !walkable_[top][right]
		|| !walkable_[bottom][left] || !walkable_[bottom][right])
	{
		return false;
	}
	
	// collision avec une autre unité
	EntityList::const_iterator it;
	sf::FloatRect other_rect;
	for (it = entities_.begin(); it != entities_.end(); ++it)
	{
		// pas de collision avec soi même
		if (*it != emitter)
		{
			(**it).GetFloorRect(other_rect);
			if (rect.Intersects(other_rect))
			{
				return false;
			}
		}
	}
	
	// ok, emitter a le droit de se déplacer
	return true;
}


/*void Zone::AddEntity(const char* name, int x, int y)
{
	sf::Vector2f position(x * Tile::SIZE, (y + 1) * Tile::SIZE);
	Entity* p = NULL;
	if (strcmp(name, "enemy") == 0)
	{
		p = new Enemy(position);
	}
	else if (strcmp(name, "pillar") == 0)
	{
		// le rectangle de surface du pillier est de 1 x 1
		sf::Vector2i floor(1 * Tile::SIZE, 1 * Tile::SIZE);
		p = new StaticItem(position, GET_IMG("pillar"), &floor);
		walkable_[y][x] = false;
	}
	else
	{
		puts(" [Zone] ajout annulé : bad entity name");
	}

	AddEntity(p);
}*/


void Zone::AddEntity(Entity* entity)
{
	entities_.push_front(entity);
}


void Zone::RemoveEntity(Entity* entity)
{
	entities_.remove(entity);
}


void Zone::AddItem(char id, int x, int y)
{
	Item* item = NULL;
	sf::Vector2f pos(x, y);
	switch (id)
	{
		case 'H':
			// Heart (coeur de vie)
			item = new Heart(pos);
			break;
		case 'R':
			// Rupee (argent)
			item = new Money(pos);
			break;
	}
	items_.push_front(item);	
}


bool Zone::GetTeleport(int x, int y, Teleporter& tp) const
{
	int key = y * WIDTH + x;
	TeleportIndexer::const_iterator it = teleporters_.find(key);
	if (it != teleporters_.end())
	{
		tp = it->second;
		return true;
	}
	return false;
}


const sf::Image* Zone::GetBackground() const
{
	return &tiles_img_;
}


void Zone::Purge()
{
	EntityList::iterator it;
	for (it = entities_.begin(); it != entities_.end(); ++it)
	{
		delete *it;
	}
	entities_.clear();
	
	ItemList::iterator it2;
	for (it2 = items_.begin(); it2 != items_.end(); ++it2)
	{
		delete *it2;
	}
	items_.clear();
}

