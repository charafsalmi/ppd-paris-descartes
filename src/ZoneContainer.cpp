#include <cassert>

#include "ZoneContainer.hpp"
#include "Game.hpp"

// durée du scrolling lors d'un changement de zone
#define SCROLL_TIME .6f

#define SCREEN_WIDTH  (Tile::SIZE * Zone::WIDTH)
#define SCREEN_HEIGHT (Tile::SIZE * Zone::HEIGHT)


ZoneContainer::ZoneContainer()
{
	width_ = 0;
	height_ = 0;
	zones_ = NULL;
	
	cds_zone_.x = 0;
	cds_zone_.y = 0;

	active_zone_ = NULL;
	next_zone_ = NULL;
}


ZoneContainer::~ZoneContainer()
{
	Unload();
}


void ZoneContainer::Load(MapName name, sf::RenderWindow& app)
{
	const char* filename = NULL;
	name_ = name;
	switch (name)
	{
		case WORLD:
			filename = "data/map/world.xml";
			break;
		case CAVES:
			filename = "data/map/caves.xml";
			break;
	}
	printf(" [ZC] loading %s...\n", filename);
	TiXmlDocument doc;
	// ouverture du fichier des zones du monde extérieur
	if (!doc.LoadFile(filename))
	{
		printf(" [ZC] échec de l'ouverture du fichier %s\n", filename);
		abort();
	}
	
	TiXmlHandle handle(&doc);
	handle = handle.FirstChildElement();
	
	// récupération des dimensions du conteneur
	const TiXmlElement* map_element = handle.Element();
	assert(map_element->QueryIntAttribute("width", &width_) == TIXML_SUCCESS);
	assert(map_element->QueryIntAttribute("height", &height_) == TIXML_SUCCESS);
	printf(" [ZC] width = %d, height = %d\n", width_, height_);

	// allocation et chargement des zones
	zones_ = new Zone* [height_];
	int count = 0;
	for (int i = 0; i < height_; ++i)
	{
		zones_[i] = new Zone [width_];
		for (int j = 0; j < width_; ++j)
		{
			TiXmlHandle zone_handle = handle.Child(count);
			if (zone_handle.Node() == NULL)
			{
				puts(" [ZoneContainer] zone manquante. abort.");
				abort();
			}
			printf(" chargement zone [%d][%d] ...\n", i, j);
			zones_[i][j].Load(zone_handle, app);
			++count;
		}
	}
	
	scrolling_ = false;
	active_zone_ = &zones_[cds_zone_.y][cds_zone_.x];
	next_zone_ = active_zone_;
	Entity::SetActiveZone(active_zone_);
}


void ZoneContainer::Unload()
{
	for (int i = 0; i < height_; ++i)
	{
		delete [] zones_[i];
	}
	delete [] zones_;
	zones_ = NULL;
}


void ZoneContainer::ChangeZone(Direction dir)
{
	int x = cds_zone_.x;
	int y = cds_zone_.y;
	
	switch (dir)
	{
		case UP:
			--y;
			break;
		case DOWN:
			++y;
			break;
		case LEFT:
			--x;
			break;
		case RIGHT:
			++x;
			break;
	}
	// est-ce qu'une zone existe aux nouvelles coordonnées ?
	if (x >= 0 && x < width_ && y >= 0 && y < height_)
	{
		printf(" [ZC] demande de changement zone en [%d][%d] acceptée\n", y, x);
		next_zone_ = &zones_[y][x];
		cds_zone_.x = x;
		cds_zone_.y = y;
		
		scrolling_ = true;
		
		scroll_.dir = dir;
		scroll_.timer = SCROLL_TIME;
		scroll_.current.SetImage(*active_zone_->GetBackground());
		scroll_.current.SetPosition(0, 0);
		scroll_.next.SetImage(*next_zone_->GetBackground());
		scroll_.next.SetPosition(0, 0);
		
		Game::GetInstance().GetPlayer()->Lock();
	}
}


bool ZoneContainer::SetActiveZone(int x, int y, bool wait)
{
	if (x >= 0 && x < width_ && y >= 0 && y < height_)
	{
		if (cds_zone_.x == x && cds_zone_.y == y)
		{
			// déjà dans la bonne zone ! (téléportation intra-zone)
			return true;
		}
		printf(" [ZC] teleportation en zone [%d][%d]\n", y, x);
		next_zone_ = &zones_[y][x];
		cds_zone_.x = x;
		cds_zone_.y = y;
		
		if (!wait)
		{
			active_zone_ = next_zone_;
			Entity::SetActiveZone(active_zone_);
		}
		return true;
	}
	puts("SetActiveZone refusée (coords invalides)");
	return false;
}


ZoneContainer::MapName ZoneContainer::GetName() const
{
	return name_;
}


void ZoneContainer::Update(float frametime)
{
	if (!scrolling_)
	{
		active_zone_->Update(frametime);
	}
	else if (scroll_.timer <= 0)
	{
		Player* player = Game::GetInstance().GetPlayer();
		switch (scroll_.dir)
		{
			case UP:
				player->SetY(SCREEN_HEIGHT - 1);
				break;
			case DOWN:
				player->SetY(player->GetFloorHeight());
				break;
			case LEFT:
				player->SetX(SCREEN_WIDTH - player->GetFloorWidth() - 1);
				break;
			case RIGHT:
				player->SetX(0);
				break;
		}
		player->Unlock();
		scrolling_ = false;
	}
	else
	{
		scroll_.timer -= frametime;
		int coord;
		Player* player = Game::GetInstance().GetPlayer();
		switch (scroll_.dir)
		{
			case UP:
				coord = SCREEN_HEIGHT - (SCREEN_HEIGHT * scroll_.timer / SCROLL_TIME);
				scroll_.current.SetY(coord);
				scroll_.next.SetY(-SCREEN_HEIGHT + coord);
				player->SetY(coord);
				break;
			case DOWN:
				coord = SCREEN_HEIGHT * scroll_.timer / SCROLL_TIME;
				scroll_.current.SetY(coord - SCREEN_HEIGHT);
				scroll_.next.SetY(coord);
				player->SetY(coord);
				break;
			case LEFT:
				coord = SCREEN_WIDTH - (SCREEN_WIDTH * scroll_.timer / SCROLL_TIME);
				scroll_.current.SetX(coord);
				scroll_.next.SetX(-SCREEN_WIDTH + coord);
				player->SetX(coord);
				break;
			case RIGHT:
				coord = SCREEN_WIDTH * scroll_.timer / SCROLL_TIME;
				scroll_.current.SetX(coord - SCREEN_WIDTH);
				scroll_.next.SetX(coord);
				player->SetX(coord);
				break;
		}
	}
}


void ZoneContainer::Show(sf::RenderWindow& app)
{
	if (scrolling_)
	{
		app.Draw(scroll_.current);
		app.Draw(scroll_.next);
		app.Draw(*Game::GetInstance().GetPlayer());
	}	
	else
	{
		// si demande de changement de zone
		if (next_zone_ != active_zone_)
		{
			active_zone_->RemoveEntity(Game::GetInstance().GetPlayer());
			
			active_zone_ = next_zone_;
			active_zone_->AddEntity(Game::GetInstance().GetPlayer());
			Entity::SetActiveZone(active_zone_);
			puts(" [ZC] changement de zone effectué");
		}
		active_zone_->Show(app);
	}
}

