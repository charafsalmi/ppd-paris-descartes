#include <fstream>
#include <cassert>
#include <algorithm>

#include <cstring>
#include <iostream>

#include "Zone.hpp"
#include "StaticItem.hpp"
#include "MediaManager.hpp"
#include "Game.hpp"


Zone::Zone()
{
	entities_qt_ = new QuadTreeNode(sf::Vector2i(0, 640), sf::Vector2i(640, 480));
}


Zone::~Zone()
{
	Purge();
}


void Zone::Load(const char* filename, sf::RenderWindow& app)
{
	static const Tileset& tileset = Tileset::GetInstance();
#ifdef DUMB_MUSIC
	short int t_music = -1;
#endif
	std::ifstream f(filename);
	assert(f); // TODO: lecture robuste et utiliser XML
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			assert(!f.eof()); // TODO: lecture robuste
			
			int tile_id;
			f >> tile_id;
			walkable_[i][j] = tileset.IsWalkable(tile_id);
			tiles_[i][j] = tile_id;
			
			// création et dessin du sprite
			sf::Sprite tile;
			tileset.MakeSprite(tile_id, tile);
			sf::Vector2f pos(j * Tile::SIZE, i * Tile::SIZE);
			tile.SetPosition(pos);
			
			Tile::Effect effect = tileset.GetEffect(tile_id);
			if (effect == Tile::HOLE)
			{
					// Etendre le format du fichier de niveau
					// -> Spécifier les arguments des trous de la zone
					// genre [i, j ] -> 8000
					// Default: -1 <-> Tomber dans le trou fait perdre une vie
					// C'est Game qui reliera 8000 à une Zone et a une Position.
					// En attendant:
				// <HACK>
				if (!strcmp(filename, "data/map/zone5.txt"))
				{
					puts("passage ajouté");
					special_args_[tile_id] = 42;
				}
			}
			
			app.Draw(tile);
		}
	}
#ifdef DUMB_MUSIC
	f >> t_music;
	zone_music_index_ = t_music;
#endif
	f.close();
	tiles_img_ = app.Capture();
	app.Clear();
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
	sf::Sprite s_tiles(tiles_img_);
	app.Draw(s_tiles);
	
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


bool Zone::CanMove(Entity* emitter, const sf::FloatRect& rect)
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
	static Entity* fake = NULL;
	std::set<Entity*> them;
	std::set<Entity*>::const_iterator it;
	
	entities_qt_->GetEntities(fake, them);

	sf::FloatRect other_rect;
	for (it = them.begin(); it != them.end(); ++it)
	{
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


void Zone::AddEntity(Entity* entity)
{
	entities_.push_front(entity);
	printf("About to add entity");
	entities_qt_->AddEntity(entity);
	puts("\t...added.");
}


void Zone::RemoveEntity(Entity* entity)
{
	entities_.remove(entity);
	entities_qt_->RemoveEntity(entity);
}


void Zone::PlaceStaticItem(int i, int j)
{
	// HACK: ajout d'un pillier

	sf::Vector2f offset(i * Tile::SIZE, (j + 1) * Tile::SIZE);
	// le rectangle de surface du pillier est de 1 x 1
	sf::Vector2i floor(1 * Tile::SIZE, 1 * Tile::SIZE);
	entities_.push_back(new StaticItem(offset, GET_IMG("pillar"), &floor));
	walkable_[j][i] = false;
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


