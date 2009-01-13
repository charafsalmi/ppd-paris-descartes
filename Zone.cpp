#include <fstream>
#include <cassert>
#include <algorithm>

#include <cstring>
#include <iostream>
#include "Zone.hpp"
#include "StaticItem.hpp"
#include "MediaManager.hpp"


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
	EntityList::iterator it;
	for (it = entities_.begin(); it != entities_.end(); ++it)
	{
		(**it).Update(frametime);
	}

	
	ItemList::iterator ti;
	for (ti = interactives_.begin(); ti != interactives_.end(); ++ti)
	{
		if ((**ti).IsDead())
		{
			puts("interactif détruit");
			delete *ti;
			ti = interactives_.erase(ti);
		}
		else
		{
//			puts("interactif pas détruit");
//			std::cerr << (**ti).value_;
		}
	}
}


void Zone::Show(sf::RenderWindow& app) const
{
	// affichage des tiles
	sf::Sprite s_tiles(tiles_img_);
	app.Draw(s_tiles);
	
	// affichage des entités
	entities_.sort(Entity::PtrComp);
	EntityList::const_iterator it;
	for (it = entities_.begin(); it != entities_.end(); ++it)
	{
		app.Draw(**it);
	}
	
	interactives_.sort(Item::PtrComp);
	ItemList::const_iterator ti;
	for (ti = interactives_.begin(); ti != interactives_.end(); ++ti)
	{
		(**ti).Blit_World(app);
	}
}


Zone::TileContent Zone::CanMove(Entity* emitter, const sf::FloatRect& rect, Item*& present)
{
	// si hors de la zone
	if (rect.Top < 0 || rect.Left < 0 || rect.Bottom > Tile::SIZE * HEIGHT
		|| rect.Right > Tile::SIZE * WIDTH)
	{
		return STATIC_NO;
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
		return STATIC_NO;
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
				return STATIC_NO;
			}
		}
	}
	
	ItemList::iterator ti;
	for (ti = interactives_.begin(); ti != interactives_.end(); ++ti)
	{
		(**ti).GetFloorRect(other_rect);
		if (rect.Intersects(other_rect))
		{
			present = &(**ti);
			return DYNAMIC_NO;
		}
	}
	
	// ok, emitter a le droit de se déplacer
	return EMPTY;
}


void Zone::AddEntity(Entity* entity)
{
	entities_.push_front(entity);
	puts("About to add entity");
	entities_qt_->AddEntity(entity);
	puts("\tAdded.");
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


void Zone::PlaceItem(char u, int i, int j)
{
	// HACK: ajout d'items qui ne bloquent pas
	
	sf::Vector2f offset(i * Tile::SIZE, (j + 1) * Tile::SIZE);
	sf::Vector2i floor(1 * Tile::SIZE, 1 * Tile::SIZE);
	
	ItemData* id = new ItemData;
	id->img_world_ = new char[8];
	strcpy(id->img_world_, "objects");

	
	Item* it = new Item (offset, *id);
	sf::IntRect i_r;
	switch (u)
	{
		case 'H':
		// Health
			it->name_ = "Heart";
			i_r = sf::IntRect(0, 0, 32, 32);			
			break;
		case 'R':
		// Rupee
			it->name_ = "Rupee";
			i_r = sf::IntRect(0, 32, 32, 64);
			break;
	}
	it->SetSubRect('w', i_r);
	it->SetPosition('w', offset);
	interactives_.push_back(it);
		
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
	
	ItemList::iterator ti;
	for (ti = interactives_.begin(); ti != interactives_.end(); ++ti)
	{
		delete *ti;
	}
}


