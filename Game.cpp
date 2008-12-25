#include "Game.hpp"

#define APP_WIDTH  (Tile::SIZE * Zone::WIDTH)
#define APP_HEIGHT (Tile::SIZE * Zone::HEIGHT)
#define APP_BPP    32
#define APP_FPS    60
#define APP_TITLE  "PPD"


Game& Game::GetInstance()
{
	static Game self;
	return self;
}


Game::Game()
{
	app_.Create(sf::VideoMode(APP_WIDTH, APP_HEIGHT, APP_BPP), APP_TITLE);
	app_.SetFramerateLimit(APP_FPS);
	
	player_ = new Player(sf::Vector2f(200, 200), *this);
	
	for (int i = 0; i < GAME_HEIGHT; ++i)
	{
		for (int j = 0; j < GAME_WIDTH; ++j)
		{
			zones_[i][j] = new Zone();
		}
	}
	
	// chargement des zones (avec ajout de quelques items)
	zones_[0][0]->Load("data/map/zone1.txt");
	zones_[0][0]->PlaceStaticItem(10, 2);
	zones_[0][0]->PlaceStaticItem(10, 4);
	zones_[0][0]->PlaceStaticItem(5, 11);
	zones_[0][0]->PlaceStaticItem(5, 10);
	zones_[0][0]->PlaceStaticItem(15, 2);
	zones_[0][0]->PlaceStaticItem(17, 2);
	zones_[0][0]->PlaceStaticItem(16, 9);
	
	zones_[0][1]->Load("data/map/zone2.txt");
	zones_[0][1]->PlaceStaticItem(9, 5);
	zones_[0][1]->PlaceStaticItem(11, 5);
	zones_[0][1]->PlaceStaticItem(9, 7);
	zones_[0][1]->PlaceStaticItem(11, 7);
	
	zones_[1][0]->Load("data/map/zone3.txt");
	
	zones_[1][1]->Load("data/map/zone4.txt");
	zones_[1][1]->PlaceStaticItem(10, 11);
	
	cds_zone_.x = 0;
	cds_zone_.y = 0;
	
	active_zone_ = NULL;
	next_zone_ = NULL;
}


Game::~Game()
{
	for (int i = 0; i < GAME_HEIGHT; ++i)
	{
		for (int j = 0; j < GAME_WIDTH; ++j)
		{
			delete zones_[i][j];
		}
	}
}


void Game::Run()
{
	sf::Event event;
	bool running = true;
	
	active_zone_ = zones_[cds_zone_.y][cds_zone_.x];
	next_zone_ = active_zone_;
	active_zone_->AddEntity(player_);
	player_->SetZone(active_zone_);
	
	float frametime;
	while (running)
	{
		while (app_.GetEvent(event))
		{
			if (event.Type == sf::Event::Closed)
			{
				running = false;
			}
		}
		frametime = app_.GetFrameTime();
		active_zone_->Update(frametime);
		active_zone_->Show(app_);
		app_.Display();
		
		// si demande de changement de zone
		if (next_zone_ != active_zone_)
		{
			active_zone_->RemoveEntity(player_);
			active_zone_ = next_zone_;
			active_zone_->AddEntity(player_);
		}
	}
	app_.Close();
}


void Game::ChangeZone(Direction dir)
{
	
	int x = cds_zone_.x;
	int y = cds_zone_.y;
	sf::Vector2f pos = player_->GetPosition();
	
	switch (dir)
	{
		case UP:
			--y;
			pos.y = Zone::HEIGHT * Tile::SIZE;
			break;
		case DOWN:
			++y;
			pos.y = 0 + player_->GetFloorHeight();
			break;
		case LEFT:
			--x;
			pos.x = (Zone::WIDTH * Tile::SIZE) - player_->GetFloorWidth();
			break;
		case RIGHT:
			++x;
			pos.x = 0;
			break;
	}
	// est-ce qu'une zone existe aux nouvelles coordonnées ?
	if (x >= 0 && x < GAME_WIDTH && y >= 0 && y < GAME_HEIGHT)
	{
		printf("-> Changement de zone en [%d][%d]\n", y, x);
		next_zone_ = zones_[y][x];
		player_->SetZone(next_zone_);	
		player_->SetPosition(pos);
		cds_zone_.x = x;
		cds_zone_.y = y;
	}
	else
	{
		puts("-> Ici s'arrête l'univers connu !");
	}
}

