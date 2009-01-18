#include <cstring>
#include <iostream>

#include "Game.hpp"
#include "Splash.hpp"
#include "Tileset.hpp"

#define APP_WIDTH  (Tile::SIZE * Zone::WIDTH)
#define APP_HEIGHT (Tile::SIZE * Zone::HEIGHT)
#define APP_BPP    32
#define APP_FPS    60
#define APP_TITLE  "PPD"

// durée du scrolling lors d'un changement de zone
#define SCROLL_TIME .6f


Game& Game::GetInstance()
{
	static Game self;
	return self;
}


Game::Game() :
	panel_(ControlPanel::GetInstance())
{

#ifdef FULLSCREEN_HACK

	DesktopMode = sf::VideoMode::GetDesktopMode();
	app_.Create(DesktopMode, APP_TITLE, sf::Style::Fullscreen);
#else
	app_.Create(sf::VideoMode(APP_WIDTH, APP_HEIGHT, APP_BPP), APP_TITLE);
#endif

	app_.SetFramerateLimit(APP_FPS);

	{
		sf::Image icon = GET_IMG("icon");
		std::cerr << icon.GetWidth() << ", " << icon.GetHeight() << "\n";
		app_.SetIcon(icon.GetWidth(), icon.GetHeight(), icon.GetPixelsPtr());
	}

	player_ = new Player(sf::Vector2f(200, 200), app_.GetInput());

	for (int i = 0; i < GAME_HEIGHT; ++i)
	{
		for (int j = 0; j < GAME_WIDTH; ++j)
		{
			zones_[i][j] = new Zone();
		}
	}
	
	// chargement des zones
	//		La classe Item caractérise ce qui permet une interaction
	//			Ex: Panneau, livre, rupee...
	//		et la classe Equipable, en héritant, 
	//		caractérise les Item (au sens de la classe Item)
	//		utilisable dans l'equipement
	//			Ex: Arc. palmes...
	zones_[0][0]->Load("data/map/zone1.xml", app_);
	zones_[0][0]->AddItem('H', 320, 192);
	zones_[0][0]->AddItem('R', 240, 200);
	
	zones_[0][1]->Load("data/map/zone2.xml", app_);
	zones_[0][1]->AddItem('H', 480, 352);
	
	zones_[0][2]->Load("data/map/zone5.xml", app_);

	zones_[1][0]->Load("data/map/zone3.xml", app_);

	zones_[1][1]->Load("data/map/zone4.xml", app_);

	zones_[1][2]->Load("data/map/zone6.xml", app_);
	
	cave_.Load("data/map/cave1.xml", app_);
	
	for (int i = 4; i <= 10; i += 2)
		for (int j = 3; j <= 9; j +=2)
			cave_.AddItem('R', i * Tile::SIZE, j * Tile::SIZE);
	
	cds_zone_.x = 0;
	cds_zone_.y = 0;

	active_zone_ = NULL;
	next_zone_ = NULL;
	
	// InGame
	on_event_meth_ = &Game::InGameOnEvent;
	show_meth_ = &Game::InGameShow;
	update_meth_ = NULL;
}


Game::~Game()
{
	app_.Close();
	for (int i = 0; i < GAME_HEIGHT; ++i)
	{
		for (int j = 0; j < GAME_WIDTH; ++j)
		{
			delete zones_[i][j];
		}
	}
	SetMusic(-1);	// What about a shared Defines.h file which would
					// state, say, "#define UNDEF" -1 ?
}


void Game::Run()
{
	sf::Event event;
	bool running = true;

	active_zone_ = zones_[cds_zone_.y][cds_zone_.x];
	next_zone_ = active_zone_;
	active_zone_->AddEntity(player_);
	Entity::SetActiveZone(active_zone_);

	float frametime;
	
#ifndef NO_SPLASH
	if (sf::PostFX::CanUsePostFX())
	{
		Splash s(app_);
		s.Run();
	}
#endif

#ifdef DUMB_MUSIC
	SetMusic(active_zone_->GetMusic());
#endif

	
	while (running)
	{
		// POLLING
		while (app_.GetEvent(event))
		{
			if (event.Type == sf::Event::Closed)
			{
				running = false;
			}
			else if (event.Type == sf::Event::KeyPressed)
			{
				if (event.Key.Code == sf::Key::F1)
				{
					puts("screen !");
					app_.Capture().SaveToFile("screenshot.png");
				}
				(this->*on_event_meth_)(event.Key.Code);
			}
		}
		// UPDATE
		frametime = app_.GetFrameTime();
		if (update_meth_ == NULL)
		{
			panel_.Update(frametime);
			active_zone_->Update(frametime);
		}
		else
		{
			(this->*update_meth_)(frametime);
		}
		
		// RENDER
		(this->*show_meth_)();
		
#ifndef FULLSCREEN_HACK		
		app_.Display();
#else
		static sf::Vector2f scal_;
		scal_.x = scal_.y = (DesktopMode.Width / 640);
		sf::Image img_(app_.Capture());
		img_.SetSmooth(true);
		sf::Sprite spr_(img_);
		spr_.SetScale(scal_);
		app_.Draw(spr_);
		app_.Display();
#endif

		// si demande de changement de zone
		if (next_zone_ != active_zone_)
		{
			active_zone_->RemoveEntity(player_);
			
			if (scroll_.need_scrolling)
			{
				// bascule en mode Scrolling
				on_event_meth_ = &Game::ScrollingOnEvent;
				update_meth_ = &Game::ScrollingUpdate;
				show_meth_ = &Game::ScrollingShow;
			}
			active_zone_ = next_zone_;
			active_zone_->AddEntity(player_);
			Entity::SetActiveZone(active_zone_);
		}
	}
}


void Game::ChangeZone(Direction dir)
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
	if (x >= 0 && x < GAME_WIDTH && y >= 0 && y < GAME_HEIGHT)
	{
		printf(" [Game] changement de zone en [%d][%d]\n", y, x);
		next_zone_ = zones_[y][x];
		cds_zone_.x = x;
		cds_zone_.y = y;
		
		scroll_.dir = dir;
		scroll_.timer = SCROLL_TIME;
		scroll_.current.SetImage(*active_zone_->GetBackground());
		scroll_.current.SetPosition(0, 0);
		scroll_.next.SetImage(*next_zone_->GetBackground());
		scroll_.next.SetPosition(0, 0);
		scroll_.need_scrolling = true;
		player_->Lock();
#ifdef DUMB_MUSIC
		SetMusic(next_zone_->GetMusic());
#endif
	}
}


void Game::Teleport(const char* zone)
{
	if (!strcmp(zone, "cave1"))
	{
		puts(" [Game] bienvenue dans la cave \\o/");
		next_zone_ = &cave_;
		scroll_.need_scrolling = false;
#ifdef DUMB_MUSIC
		SetMusic(cave_.GetMusic());
#endif
	}
}


#ifdef DUMB_MUSIC
void Game::SetMusic(short val)
{
	static Music* music_ = NULL;
	static short current_music_index_ = -1;

	std::cerr << " [Game] SetMusic\tCur: " << current_music_index_ << "\t New: " << val << "\n";
	
	if (val > 0 && val != current_music_index_)
	{
		if (music_ != NULL)
		{
			music_->Stop();
			delete music_;
			music_ = NULL;
		}
		music_ = GET_MUSIC(val);
		music_->Play();
		current_music_index_ = val;
	}
	else if (val == -1 && music_ != NULL)	// -1 <=> NO_MUSIC.
	{
		music_->Stop();
		delete music_;
		music_ = NULL;
	}
	current_music_index_ = val;
}
#endif


// méthodes de spécialisation du comportement

void Game::InGameOnEvent(sf::Key::Code key)
{
	if (key == sf::Key::Return)
	{
		puts("mode inventory");
		// bascule en mode Inventory
		on_event_meth_ = &Game::InventoryOnEvent;
		show_meth_ = &Game::InventoryShow;
		player_->Lock();
	}
	player_->OnEvent(key);
}


void Game::InGameShow()
{
	active_zone_->Show(app_);
	panel_.Show(app_);
}


void Game::InventoryOnEvent(sf::Key::Code key)
{
	if (key == sf::Key::Return)
	{
		puts("mode ingame");
		// bascule en mode InGame
		on_event_meth_ = &Game::InGameOnEvent;
		show_meth_ = &Game::InGameShow;
		player_->Unlock();
	}
	panel_.GetInventory()->OnEvent(key);
}


void Game::InventoryShow()
{
	active_zone_->Show(app_);
	panel_.GetInventory()->Show(app_);
}


void Game::ScrollingOnEvent(sf::Key::Code key)
{
	(void) key;
}


void Game::ScrollingUpdate(float frametime)
{
	if (scroll_.timer <= 0)
	{
		// bascule en mode InGame
		on_event_meth_ = &Game::InGameOnEvent;
		update_meth_ = NULL;
		show_meth_ = &Game::InGameShow;
		
		switch (scroll_.dir)
		{
			case UP:
				player_->SetY(APP_HEIGHT - 1);
				break;
			case DOWN:
				player_->SetY(player_->GetFloorHeight());
				break;
			case LEFT:
				player_->SetX(APP_WIDTH - player_->GetFloorWidth() - 1);
				break;
			case RIGHT:
				player_->SetX(0);
				break;
		}
		player_->Unlock();
	}
	else
	{
		scroll_.timer -= frametime;
		int coord;
		switch (scroll_.dir)
		{
			case UP:
				coord = APP_HEIGHT - (APP_HEIGHT * scroll_.timer / SCROLL_TIME);
				scroll_.current.SetY(coord);
				scroll_.next.SetY(-APP_HEIGHT + coord);
				player_->SetY(coord);
				break;
			case DOWN:
				coord = APP_HEIGHT * scroll_.timer / SCROLL_TIME;
				scroll_.current.SetY(coord - APP_HEIGHT);
				scroll_.next.SetY(coord);
				player_->SetY(coord);
				break;
			case LEFT:
				coord = APP_WIDTH - (APP_WIDTH * scroll_.timer / SCROLL_TIME);
				scroll_.current.SetX(coord);
				scroll_.next.SetX(-APP_WIDTH + coord);
				player_->SetX(coord);
				break;
			case RIGHT:
				coord = APP_WIDTH * scroll_.timer / SCROLL_TIME;
				scroll_.current.SetX(coord - APP_WIDTH);
				scroll_.next.SetX(coord);
				player_->SetX(coord);
				break;
		}
	}
}


void Game::ScrollingShow()
{
	app_.Draw(scroll_.current);
	app_.Draw(scroll_.next);
	app_.Draw(*player_);
	panel_.Show(app_);
}


