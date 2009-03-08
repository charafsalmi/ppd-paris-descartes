#include <cassert>

#include "ZoneContainer.hpp"
#include "Game.hpp"

// durée du scrolling lors d'un changement de zone
#define SCROLL_TIME .6f


ZoneContainer::ZoneContainer()
{
	width_ = 0;
	height_ = 0;
	zones_ = NULL;
	scrolling_ = false;
	cds_zone_.x = -1; // undefined coords
	cds_zone_.y = -1;

	active_zone_ = NULL;
	next_zone_ = NULL;
}


ZoneContainer::~ZoneContainer()
{
	Unload();
}


void ZoneContainer::Load(MapName name)
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
	// ouverture du fichier des zones du monde extérieur
	if (!xml_doc_.LoadFile(filename))
	{
		printf(" [ZC] échec de l'ouverture du fichier %s\n", filename);
		abort();
	}

	TiXmlHandle handle(&xml_doc_);
	handle = handle.FirstChildElement();

	// récupération des dimensions du conteneur
	const TiXmlElement* map_element = handle.Element();
	assert(map_element->QueryIntAttribute("width", &width_) == TIXML_SUCCESS);
	assert(map_element->QueryIntAttribute("height", &height_) == TIXML_SUCCESS);
	printf(" [ZC] width = %d, height = %d\n", width_, height_);

	// allocation des zones
	zones_ = new Zone* [height_];
	for (int i = 0; i < height_; ++i)
	{
		zones_[i] = new Zone [width_];
		for (int j = 0; j < width_; ++j)
		{
			printf(" [ZC] zone [%d][%d] allouee...\n", i, j);
		}
	}

	// on charge la première zone de la map
	SetActiveZone(0, 0, false);
}


void ZoneContainer::Unload()
{
	for (int i = 0; i < height_; ++i)
	{
		delete [] zones_[i];
	}
	delete [] zones_;
	zones_ = NULL;
	active_zone_ = NULL;
	next_zone_ = NULL;
	cds_zone_.x = -1;
	cds_zone_.y = -1;
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
	if (SetActiveZone(x, y))
	{
		scrolling_ = true;
		scroll_.dir = dir;
		scroll_.timer = SCROLL_TIME;
		scroll_.current.SetImage(*active_zone_->GetBackground());
		scroll_.current.SetPosition(0, 0);
		scroll_.current.FlipY(true); // HACK BUGFIX SFML 1.4
		scroll_.next.SetImage(*next_zone_->GetBackground());
		scroll_.next.SetPosition(0, 0);
		scroll_.next.FlipY(true); // HACK BUGFIX SFML 1.4

		Game::GetInstance().GetPlayer()->Lock();
		printf(" [ZC] scrolling vers la zone [%d][%d]\n", y, x);
	}
}


bool ZoneContainer::SetActiveZone(int x, int y, bool wait)
{
	// est-ce qu'une zone existe aux nouvelles coordonnées ?
	if (x >= 0 && x < width_ && y >= 0 && y < height_)
	{
		if (cds_zone_.x == x && cds_zone_.y == y)
		{
			printf(" [ZC] déjà en zone [%d][%d], rien à faire\n", y, x);
			return true;
		}
		next_zone_ = &zones_[y][x];
		cds_zone_.x = x;
		cds_zone_.y = y;

		// est-ce la prochaine zone est déjà chargée ?
		if (!next_zone_->IsLoaded())
		{
			int offset = y * width_ + x;
			TiXmlHandle handle(&xml_doc_);
			handle = handle.FirstChildElement().Child(offset);
			if (handle.Node() == NULL)
			{
				printf(" [ZC] impossible de charger la zone [%d][%d]\n", y, x);
				abort();
			}
			printf(" [ZC] chargement de la zone [%d][%d]\n", y, x);
			next_zone_->Load(handle);
		}

		// changement immédiat ?
		if (!wait)
		{
			scrolling_ = false;
			active_zone_ = next_zone_;
			Entity::SetActiveZone(active_zone_);
			printf(" [ZC] [%d][%d] est maintenant la zone active\n", y, x);
		}

		return true;
	}
	printf(" [ZC] coordonnées de la prochaine zone [%d][%d] invalides\n", y, x);
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
				player->SetY(Zone::HEIGHT_PX - 1);
				break;
			case DOWN:
				player->SetY(player->GetFloorHeight());
				break;
			case LEFT:
				player->SetX(Zone::WIDTH_PX - player->GetFloorWidth() - 1);
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
				coord = (int)(Zone::HEIGHT_PX - (Zone::HEIGHT_PX * scroll_.timer / SCROLL_TIME));
				scroll_.current.SetY(coord);
				scroll_.next.SetY(-Zone::HEIGHT_PX + coord);
				player->SetY(coord);
				break;
			case DOWN:
				coord = (int)(Zone::HEIGHT_PX * scroll_.timer / SCROLL_TIME);
				scroll_.current.SetY(coord - Zone::HEIGHT_PX);
				scroll_.next.SetY(coord);
				player->SetY(coord);
				break;
			case LEFT:
				coord = (int)(Zone::WIDTH_PX - (Zone::WIDTH_PX * scroll_.timer / SCROLL_TIME));
				scroll_.current.SetX(coord);
				scroll_.next.SetX(-Zone::WIDTH_PX + coord);
				player->SetX(coord);
				break;
			case RIGHT:
				coord = (int)(Zone::WIDTH_PX * scroll_.timer / SCROLL_TIME);
				scroll_.current.SetX(coord - Zone::WIDTH_PX);
				scroll_.next.SetX(coord);
				player->SetX(coord);
				break;
		}
	}
}


void ZoneContainer::Render(sf::RenderTarget& target) const
{
	if (scrolling_)
	{
		target.Draw(scroll_.next);
		target.Draw(scroll_.current);
		target.Draw(*Game::GetInstance().GetPlayer());
	}
	else
	{
		// si demande de changement de zone différée
		if (next_zone_ != active_zone_)
		{
			active_zone_->RemoveEntity(Game::GetInstance().GetPlayer());
			active_zone_ = next_zone_;
			active_zone_->AddEntity(Game::GetInstance().GetPlayer());
			Entity::SetActiveZone(active_zone_);
		}
		else
		{
			active_zone_->Show(target);
		}
	}
}

