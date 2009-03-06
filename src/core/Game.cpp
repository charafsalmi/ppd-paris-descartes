#include <cstring>
#include <iostream>

#include "Game.hpp"
#include "../misc/MediaManager.hpp"
#include "../misc/ConfigParser.hpp"
#include "../gui/Splash.hpp"
#include "../xml/tinyxml.h"

#define APP_WIDTH  Zone::WIDTH_PX
#define APP_HEIGHT (Zone::HEIGHT_PX + ControlPanel::HEIGHT_PX)
#define APP_BPP    32
#define APP_FPS    60
#define APP_TITLE  "PPD"

#define CONFIG_FILE "config/config.css"

Game& Game::GetInstance()
{
	static Game self;
	return self;
}


Game::Game() :
	panel_(ControlPanel::GetInstance())
{
	app_.Create(sf::VideoMode(APP_WIDTH, APP_HEIGHT, APP_BPP), APP_TITLE);
	app_.SetFramerateLimit(APP_FPS);

	const sf::Image& icon = GET_IMG("icon");
	app_.SetIcon(icon.GetWidth(), icon.GetHeight(), icon.GetPixelsPtr());
}


Game::~Game()
{
	// writing options
	ConfigParser config;
	config.SeekSection("Settings");
	config.WriteItem("panel_on_top", options_.panel_on_top ? 1 : 0);
	
	config.SaveToFile(CONFIG_FILE);
	
	app_.Close();

#ifdef DUMB_MUSIC
	SetMusic(-1);
#endif
}


void Game::Init()
{
	player_ = new Player(sf::Vector2f(300, 300), app_.GetInput());

    #ifdef WINDOW_TEST
    fen_.Load("data/window/test.xml");
    #endif
    
	// chargement du conteneur de zones
	zone_container_.Load(ZoneContainer::WORLD);
	zone_container_.SetPosition(0, ControlPanel::HEIGHT_PX);
	
	// default options
	options_.panel_on_top = true;
	
	// load options
	ConfigParser config;
	if (config.LoadFromFile(CONFIG_FILE))
	{
		printf("loading %s...\n", CONFIG_FILE);
		config.SeekSection("Settings");
		config.ReadItem("panel_on_top", options_.panel_on_top);
		if (!options_.panel_on_top)
		{
			InGameOnEvent(sf::Key::PageDown);
		}
	}
	
	// InGame
	SetMode(IN_GAME);
}


void Game::Run()
{
	sf::Event event;
	bool running = true;
	float frametime;
	zone_container_.GetActiveZone()->AddEntity(player_);

#ifndef NO_SPLASH
	Splash s(app_);
	s.Run();
#endif

#ifdef DUMB_MUSIC
	SetMusic(active_zone_->GetMusic());
#endif

	while (running)
	{
		// POLLING
		while (app_.GetEvent(event))
		{
#ifdef WINDOW_TEST
			fen_.ManageEvent(event);
#endif
			if (event.Type == sf::Event::Closed)
			{
				running = false;
			}
			else if (event.Type == sf::Event::KeyPressed)
			{
				switch (event.Key.Code)
				{
					case sf::Key::F1:
						TakeScreenshot("screenshot");
						break;
					case sf::Key::Escape:
						running = false;
						break;
					default:
						(this->*on_event_meth_)(event.Key.Code);
						break;
				}
			}
		}
		// UPDATE
		frametime = app_.GetFrameTime();
		(this->*update_meth_)(frametime);

		// RENDER
		(this->*render_meth_)();
		app_.Display();

		if (zone_container_.GetName() != next_map_name_)
		{
			ChangeZoneContainer(next_map_name_);
		}
	}
}


void Game::TakeScreenshot(const char* directory)
{
	char currentTime[256];
	std::string filename;
	time_t t = time(NULL);
	
	strftime(currentTime, sizeof(currentTime), "%d-%m-%Y-%M%S", localtime(&t));
	
	filename = str_sprintf("%s/%s.png", directory, currentTime, t);
	
	app_.Capture().SaveToFile(filename);
	std::cout << "Screenshot " << filename << " taken" << std::endl;
}


void Game::ChangeZone(ZoneContainer::Direction direction)
{
	zone_container_.ChangeZone(direction);
}


void Game::ChangeZoneContainer(ZoneContainer::MapName map_name)
{
	puts(" [Game] changement de map");
	// on retire le joueur de la zone de l'ancien conteneur
	Zone* active = zone_container_.GetActiveZone();
	active->RemoveEntity(player_);
	zone_container_.Unload();
	zone_container_.Load(map_name);
	if (!zone_container_.SetActiveZone(next_zone_cds_.x, next_zone_cds_.y, false))
	{
		puts(" impossible d'activer la zone du conteneur cible");
		abort();
	}
	// insertion du joueur dans le nouveau conteneur
	active = zone_container_.GetActiveZone();
	active->AddEntity(player_);

	next_map_name_ = map_name;
}


void Game::Teleport(const Zone::Teleporter& tp)
{
	ZoneContainer::MapName map_name = (ZoneContainer::MapName) tp.zone_container;
	if (map_name != zone_container_.GetName())
	{
		puts(" [Game] téléportation inter-conteneurs");
		next_map_name_ = map_name;
		next_zone_cds_ = tp.zone_coords;
	}
	else
	{
		puts(" [Game] téléportation intra-conteneur");
		// on informe le conteneur que la zone active doit changer à la prochaine itération
		if (!zone_container_.SetActiveZone(tp.zone_coords.x, tp.zone_coords.y))
		{
			puts("bad coords");
			abort();
		}
	}

	int x = tp.tile_coords.x * Tile::SIZE;
	int y = tp.tile_coords.y * Tile::SIZE;
	player_->SetPosition(x, y);
}


#ifdef DUMB_MUSIC
void Game::SetMusic(int value)
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

void Game::SetMode(Mode mode)
{
	// initialisation des callbacks
	switch (mode)
	{
		case IN_GAME:
			puts("mode ingame");
			on_event_meth_ = &Game::InGameOnEvent;
			update_meth_ = &Game::DefaultUpdate;
			render_meth_ = &Game::InGameShow;
			player_->Unlock();
			break;
		case INVENTORY:
			puts("mode inventory");
			on_event_meth_ = &Game::InventoryOnEvent;
			update_meth_ = &Game::DefaultUpdate;
			render_meth_ = &Game::InventoryShow;
			player_->Lock();
			break;
	}
}

// méthodes callbacks

void Game::DefaultUpdate(float frametime)
{
	panel_.Update(frametime);
	zone_container_.Update(frametime);
}


void Game::InGameOnEvent(sf::Key::Code key)
{
	if (zone_container_.Scrolling())
	{
		return;
	}
	switch (key)
	{
		case sf::Key::Return:
			SetMode(INVENTORY);
			break;
		case sf::Key::PageUp:
			panel_.SetPosition(0, 0);
			zone_container_.SetPosition(0, ControlPanel::HEIGHT_PX);
			options_.panel_on_top = true;
			break;
		case sf::Key::PageDown:
			panel_.SetPosition(0, Zone::HEIGHT_PX);
			zone_container_.SetPosition(0, 0);
			options_.panel_on_top = false;
			break;
		default:
			player_->OnEvent(key);
			break;
	}
}


void Game::InGameShow()
{
	app_.Draw(zone_container_);
	app_.Draw(panel_);
#ifdef WINDOW_TEST
	fen_.Show(app_);
#endif
}


void Game::InventoryOnEvent(sf::Key::Code key)
{
	if (key == sf::Key::Return)
	{
		SetMode(IN_GAME);
	}
	panel_.GetInventory()->OnEvent(key);
}


void Game::InventoryShow()
{
	app_.Draw(zone_container_);
	panel_.GetInventory()->Show(app_);
}

