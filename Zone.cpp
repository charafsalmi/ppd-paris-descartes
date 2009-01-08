#include <fstream>
#include <cassert>
#include <algorithm>

#include "Zone.hpp"
#include "StaticItem.hpp"
#include "Tileset.hpp"
#include "MediaManager.hpp"


Zone::Zone()
{
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
			
			// création et dessin du sprite
			sf::Sprite tile;
			tileset.MakeSprite(tile_id, tile);
			sf::Vector2f pos(j * Tile::SIZE, i * Tile::SIZE);
			tile.SetPosition(pos);
			
			app.Draw(tile);
		}
	}
#ifdef DUMB_MUSIC	
	f >> t_music;
	zone_music_index_ = t_music;
#endif
	f.close();
	tiles_ = app.Capture();
	app.Clear();
}


void Zone::Update(float frametime)
{
	EntityList::iterator it;
	for (it = entities_.begin(); it != entities_.end(); ++it)
	{
		(**it).Update(frametime);
	}
}


void Zone::Show(sf::RenderWindow& app) const
{
	// affichage des tiles
	sf::Sprite s_tiles(tiles_);
	app.Draw(s_tiles);
	
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
}


void Zone::RemoveEntity(Entity* entity)
{
	entities_.remove(entity);
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


void Zone::Purge()
{
	EntityList::iterator it;
	for (it = entities_.begin(); it != entities_.end(); ++it)
	{
		delete *it;
	}
	entities_.clear();
}


