#include <algorithm>
#include <iostream>
#include <sstream>
#include <cassert>
#include <cstring>

#include "Zone.hpp"
#include "ZoneContainer.hpp"
#include "Game.hpp"
#include "../entities/EntityFactory.hpp"
#include "../entities/Player.hpp"
#include "../misc/MediaManager.hpp"
#include "../gui/ControlPanel.hpp"

#define ZONE_SUBRECT sf::IntRect(0, ControlPanel::HEIGHT_PX, WIDTH_PX, ControlPanel::HEIGHT_PX + HEIGHT_PX)


Zone::Zone()
{
	loaded_ = false;
	last_id_ = 1;
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

			if (tileset.IsAnimated(tile_id))
			{
				Tileset::AnimatedTile animated_tile;
				tileset.MakeAnimatedTile(tile_id, animated_tile);
				animated_tile.sprite.SetPosition(j * Tile::SIZE, i * Tile::SIZE);
				animated_.push_back(animated_tile);
			}

			/* on dessine toutes les tiles dans le buffer de la fenêtre de rendu
			ce buffer sera ensuite capturé dans tiles_img_ afin de pouvoir
			afficher toutes les tiles en un seul coup
			(hack de rendu sur image, en attendant SFML 2.0)
			*/
			app.Draw(tile);
		}
	}

	// chargement des entités
	EntityFactory& factory = EntityFactory::GetInstance();
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
			Entity* entity = factory.BuildUnit(id, sf::Vector2f(x, y));
			if (entity != NULL)
			{
				AddEntity(entity);
			}
		}
		else
		{
		    OutputW << ZONE_S << "Entite invalide (ignore)" << lEnd;
		}
		elem = elem->NextSiblingElement();
	}

	// chargements des items
	elem = handle.FirstChildElement("items").FirstChildElement().Element();
	while (elem != NULL)
	{
		bool ok = true;
		int id, x, y;
		ok &= (elem->QueryIntAttribute("id", &id) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("x", &x) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("y", &y) == TIXML_SUCCESS);
		if (ok)
		{
			AddItem(id, x, y);
		}
		else
		{
		    OutputW << ZONE_S << "Item invalide (ignore)" << lEnd;
		}
		elem = elem->NextSiblingElement();
	}

	// chargement des décors
	elem = handle.FirstChildElement("decors").FirstChildElement().Element();
	while (elem != NULL)
	{
		bool ok = true;
		int id, x, y;
		ok &= (elem->QueryIntAttribute("id", &id) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("x", &x) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("y", &y) == TIXML_SUCCESS);

		if (ok)
		{
			++y;
			AddDecor(factory.BuildDecor(id, sf::Vector2i(x, y)), x, y);
		}
		else
		{
		    OutputW << ZONE_S << "Decor invalide (ignore)" << lEnd;
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
		    OutputW << ZONE_S << "Teleporteur invalide (ignore)" << lEnd;
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
	if (!tiles_img_.CopyScreen(app, ZONE_SUBRECT))
	{
	    OutputW << ZONE_S << "Echec de creation image des tiles" << lEnd;
	}
	tiles_sprite_.SetImage(tiles_img_);
	tiles_sprite_.FlipY(true); // HACK BUGFIX SFML 1.4
	app.Clear();
	loaded_ = true;
}


bool Zone::IsLoaded() const
{
	return loaded_;
}


void Zone::Update(float frametime)
{
	// updating animated tiles
	static const Tileset& tileset = Tileset::GetInstance();
	if (tileset.NeedUpdate(frametime))
	{
		for (std::vector<Tileset::AnimatedTile>::iterator it = animated_.begin();
			it != animated_.end(); ++it)
		{
			tileset.UpdateAnimated(*it);
		}
	}

	// removing dead entities
	EntityList::iterator it1, it2, it_end;

	for (it1 = entities_.begin(); it1 != entities_.end();)
	{
		if ((**it1).IsDead())
		{
			delete *it1;
			it1 = entities_.erase(it1);
		}
		else
		{
			++it1;
		}
	}

	// collisions avec les entités
	it_end = entities_.end();
	sf::FloatRect rect1, rect2;
	for (it1 = entities_.begin(); it1 != it_end; ++it1)
	{
		(**it1).Update(frametime);
		(**it1).GetCollideRect(rect1);
		it2 = it1;
		for (++it2; it2 != it_end; ++it2)
		{
			(**it2).GetCollideRect(rect2);
			if (rect1.Intersects(rect2))
			{
				(**it1).OnCollide(**it2);
				(**it2).OnCollide(**it1);
			}
		}
	}

	// collisions avec les items
	ItemList::iterator it3;
	Player* player = Game::GetInstance().GetPlayer();
	sf::FloatRect player_rect, item_rect;
	player->GetFloorRect(player_rect);

	for (it3 = items_.begin(); it3 != items_.end();)
	{
		if ((**it3).IsDead())
		{
			delete *it3;
			it3 = items_.erase(it3);
		}
		else
		{
			(**it3).GetFloorRect(item_rect);
			if (item_rect.Intersects(player_rect))
			{
				(**it3).OnCollide(*player);
				break;
			}
			++it3;
		}
	}
}


bool Zone::CanMove(const sf::FloatRect& rect) const
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

	// ok, emitter a le droit de se déplacer
	return true;
}


void Zone::AddDecor(Entity* decor, int x, int y)
{
	// récupérer les dimensions en nombre de tiles
	int right_x = x + decor->GetFloorWidth() / Tile::SIZE;
	int top_y = y - decor->GetFloorHeight() / Tile::SIZE;
	int xcopy = x;
	for (--y; y >= top_y; --y)
	{
		for (x = xcopy; x < right_x; ++x)
		{
			walkable_[y][x] = false;
		}
	}
	AddEntity(decor);
}


void Zone::AddEntity(Entity* entity)
{
	entity->SetID(last_id_++);
	entities_.push_front(entity);
}


void Zone::RemoveEntity(Entity* entity)
{
	entities_.remove(entity);
}


void Zone::AddItem(int id, int x, int y)
{
	ControlPanel& panel = ControlPanel::GetInstance();
	if(!panel.GetInventory()->HasItem(id))
	{
		Item* item = EntityFactory::GetInstance().BuildItem(id, sf::Vector2f(x, y));
		item->SetID(last_id_++);
		items_.push_front(item);
	}
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


void Zone::Render(sf::RenderTarget& target) const
{
	// affichage des tiles
	target.Draw(tiles_sprite_);

	// affichage des tiles animées
	for (std::vector<Tileset::AnimatedTile>::const_iterator it = animated_.begin();
		it != animated_.end(); ++it)
	{
		target.Draw(it->sprite);
	}

	// affichage des items
	ItemList::const_iterator it2;
	for (it2 = items_.begin(); it2 != items_.end(); ++it2)
	{
		target.Draw(**it2);
	}

	// affichage des entités
	entities_.sort(Entity::PtrComp);
	EntityList::const_iterator it;
	for (it = entities_.begin(); it != entities_.end(); ++it)
	{
		target.Draw(**it);
	}
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

	animated_.clear();
}

