#include <algorithm>
#include <sstream>
#include <cassert>
#include <cstring>

#include "Zone.hpp"
#include "ZoneContainer.hpp"
#include "Game.hpp"
#include "../entities/EntityFactory.hpp"
#include "../entities/Player.hpp"
#include "../entities/Hit.hpp"
#include "../entities/Decor.hpp"
#include "../entities/Item.hpp"
#include "../misc/MediaManager.hpp"
#include "../misc/Log.hpp"

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
	// chargement de la musique
	const char* p = handle.ToElement()->Attribute("music");
	if (p == NULL)
	{
		puts("warning: missiing 'music' attribute");
	}
	else
	{
		music_name_ = p;
	}

	// chargement des tiles
	sf::RenderWindow& app = Game::GetInstance().GetApp();
	static const Tileset& tileset = Tileset::GetInstance();
	const TiXmlElement* elem = handle.FirstChildElement("tiles").Element();

	std::istringstream all_tiles(elem->GetText());
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			if (all_tiles.eof())
			{
				i = HEIGHT;
				j = WIDTH;
				puts("warning: tiles are missing");
			}

			int tile_id;
			all_tiles >> tile_id;
			walkable_[i][j] = tileset.GetEffect(tile_id);
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

	// chargement des mobs
	EntityFactory& factory = EntityFactory::GetInstance();
	elem = handle.FirstChildElement("mobs").FirstChildElement().Element();
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
		    puts("warning: can't parse mob");
		}
		elem = elem->NextSiblingElement();
	}

	// chargements des items
	elem = handle.FirstChildElement("items").FirstChildElement().Element();
	while (elem != NULL)
	{
		bool ok = true;
		int x, y;
		const char* name;
		ok &= ((name = elem->Attribute("name")) != NULL);
		ok &= (elem->QueryIntAttribute("x", &x) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("y", &y) == TIXML_SUCCESS);
		if (ok)
		{
			Item::Type type = Item::StringToType(name);
			AddItem(type, x, y);
		}
		else
		{
		    puts("warning: can't parse item");
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
		    puts("warning: can't parse decor");
		}
		elem = elem->NextSiblingElement();
	}

	// chargement des téléporteurs
	elem = handle.FirstChildElement("teleporters").FirstChildElement().Element();
	while (elem != NULL)
	{
		bool ok = true;
		int x, y, zone_x, zone_y, tile_x, tile_y;
		// coordonnées tile du téléporteur
		ok &= (elem->QueryIntAttribute("x", &x) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("y", &y) == TIXML_SUCCESS);
		// carte cible
		const char* attr_map = elem->Attribute("map");
		ok &= (attr_map != NULL);

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
			tp.map_name = attr_map;
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
		puts("warning: can't copy tiles image from screen");
	}
	tiles_sprite_.SetImage(tiles_img_);
	tiles_sprite_.FlipY(true); // HACK BUGFIX SFML 1.4, 1.5
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
	sf::FloatRect rect1, rect2, overlap;
	for (it1 = entities_.begin(); it1 != it_end; ++it1)
	{
		(**it1).Update(frametime);
		it2 = it1;
		for (++it2; it2 != it_end; ++it2)
		{
			// récupération des surfaces de collision
			if ((**it1).CanFloorCollide() && (**it2).CanFloorCollide())
			{
				(**it1).GetFloorRect(rect1);
				(**it2).GetFloorRect(rect2);
			}
			else
			{
				(**it1).GetGlobalRect(rect1);
				(**it2).GetGlobalRect(rect2);
			}

			if (rect1.Intersects(rect2, &overlap))
			{
				(**it1).OnCollide(**it2, overlap);
				(**it2).OnCollide(**it1, overlap);
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


bool Zone::CanMove(const sf::FloatRect& rect, int tileflag) const
{
	// si hors de la zone
	if (rect.Top < 0 || rect.Left < 0 || rect.Bottom > Tile::SIZE * HEIGHT
		|| rect.Right > Tile::SIZE * WIDTH)
	{
		return false;
	}
	// on calcule les indices de chaque coin, avec un décalage de 1px pour
	// éviter une collision lorsque l'entité est simplement collé à la tile
	int left = (int) rect.Left / Tile::SIZE;
	int top = (int) rect.Top / Tile::SIZE;
	int right = (int) (rect.Right - 1) / Tile::SIZE;
	int bottom = (int) (rect.Bottom - 1) / Tile::SIZE;

	// si collision avec une tile non accpetée
	if (!(walkable_[top][left] & tileflag)
		|| !(walkable_[top][right] & tileflag)
		|| !(walkable_[bottom][left] & tileflag)
		|| !(walkable_[bottom][right] & tileflag))
	{
		return false;
	}

	// ok, emitter a le droit de se déplacer
	return true;
}


void Zone::AddDecor(Entity* decor, int x, int y)
{
	// indexation des décors désactivée sur la matrice walkable afin
	// de gére les décors mobiles
	// éventuellement, indexer seulement les décors fixes
	// récupérer les dimensions en nombre de tiles
	/*int right_x = x + decor->GetFloorWidth() / Tile::SIZE;
	int top_y = y - decor->GetFloorHeight() / Tile::SIZE;
	int xcopy = x;
	for (--y; y >= top_y; --y)
	{
		for (x = xcopy; x < right_x; ++x)
		{
			walkable_[y][x] = Tile::BLOCK;
		}
	}*/

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


void Zone::ClearHits()
{
	EntityList::iterator it;
	for (it = entities_.begin(); it != entities_.end();)
	{
		if (dynamic_cast<Hit*>(*it) != NULL)
		{
			it = entities_.erase(it);
		}
		else
		{
			++it;
		}
	}
}


void Zone::AddItem(Item::Type type, int x, int y)
{
	Item* item = EntityFactory::GetInstance().BuildItem(type, sf::Vector2f(x, y));
	item->SetID(last_id_++);
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


void Zone::Interact(Entity* player)
{
#define MAX_DIST 50
	EntityList::iterator it;
	Entity* nearest = NULL;
	float min_dist = MAX_DIST;
	for (it = entities_.begin(); it != entities_.end(); ++it)
	{
		if ((**it).CanInteract() && *it != player)
		{
			float dist = player->Distance(**it);
			if (dist < min_dist)
			{
				nearest = *it;
				min_dist = dist;
			}
		}
	}
	if (nearest != NULL)
	{
		printf("nearest is at dist %.1f)\n", min_dist);
	}
}

