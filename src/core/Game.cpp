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
#define KEY_PAUSE   sf::Key::F11


Game& Game::GetInstance()
{
	static Game self;
	return self;
}


Game::Game() :
	panel_(ControlPanel::GetInstance()),
	message_(GET_BITMAP_FONT("retro"))
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
#ifndef NO_SPLASH
	Splash s(app_);
	s.Run();
#endif

	player_ = new Player(sf::Vector2f(300, 300), app_.GetInput());

	// chargement du conteneur de zones
	zone_container_.Load(ZoneContainer::WORLD);
	next_map_name_ = ZoneContainer::WORLD;

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
			//InGameOnEvent(sf::Key::PageDown);
		}
	}

	// InGame
	SetMode(IN_GAME);
}


void Game::Run()
{
	sf::Event event;
	float frametime;
	zone_container_.GetActiveZone()->AddEntity(player_);

    running_ = true;

#ifdef DUMB_MUSIC
	SetMusic(active_zone_->GetMusic());
#endif

	while (running_)
	{
		// POLLING
		while (app_.GetEvent(event))
		{
			(this->*on_event_meth_)(event);

			// global events
			if (event.Type == sf::Event::Closed)
			{
				running_ = false;
			}
			else if (event.Type == sf::Event::KeyPressed)
			{
				switch (event.Key.Code)
				{
					case sf::Key::F1:
						TakeScreenshot("screenshot");
						break;
					case sf::Key::Escape:
						running_ = false;
						break;
					default:
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


void Game::EndGame()
{
	message_.SetText("Tu n'es pas le digne fils de Chuck Norris (Enter pour rejouer)");
	message_.SetPosition(10, 250);
	SetMode(GAME_OVER);

}


void Game::SetMode(Mode mode)
{
	// On sauvegarde le mode
	//mode_ = mode; (inutile, je laisse si jamais ça devrait changer)

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
		case GAME_OVER:
			puts("mode gameover");
			on_event_meth_ = &Game::GameOverOnEvent;
			update_meth_ = &Game::GameOverUpdate;
			render_meth_ = &Game::GameOverShow;
			break;
        case PAUSE:
            puts("Mode pause");
            on_event_meth_ = &Game::PauseOnEvent;
			update_meth_ = &Game::PauseUpdate;
			render_meth_ = &Game::PauseShow;
            break;
	}
}

// méthodes callbacks

void Game::DefaultUpdate(float frametime)
{
	panel_.Update(frametime);
	zone_container_.Update(frametime);
}

// IN_GAME

void Game::InGameOnEvent(const sf::Event& event)
{
	sf::Key::Code key = event.Key.Code;
#ifdef WINDOW_TEST
	fen_.ManageEvent(event);
#endif

	if (event.Type == sf::Event::KeyPressed)
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
			case KEY_PAUSE:
				SetMode(PAUSE);
				break;
			default:
				player_->OnEvent(key);
				break;
		}
	}
}


void Game::InGameShow()
{
	app_.Draw(zone_container_);
	app_.Draw(panel_);

	if (zone_container_.GetName() != next_map_name_)
	{
		ChangeZoneContainer(next_map_name_);
	}
#ifdef WINDOW_TEST
	app_.Draw(fen_);
#endif
}

// INVENTORY

void Game::InventoryOnEvent(const sf::Event& event)
{
    // TODO: Evenement à déporter dans le gestionnaire de fenêtre
	if ((event.Key.Code == sf::Key::Return) && (event.Type == sf::Event::KeyPressed))
		SetMode(IN_GAME);

	if(panel_.GetInventory()->ManageEvent(event) == WinInventory::_CLOSE)
	    SetMode(IN_GAME);
}


void Game::InventoryShow()
{
	app_.Draw(zone_container_);
	app_.Draw(panel_);
	app_.Draw(*panel_.GetInventory());
}

// PAUSE

void Game::PauseOnEvent(const sf::Event& event)
{
	if (event.Type == sf::Event::KeyPressed)
	{
		if (event.Key.Code == KEY_PAUSE)
		{
			SetMode(IN_GAME);
			return;
		}
	}
	if (pause_.ManageEvent(event) == WinPause::_EXIT)
		running_ = false;
}


void Game::PauseUpdate(float)
{

}


void Game::PauseShow()
{
	app_.Draw(pause_);
}


// GAME_OVER

void Game::GameOverOnEvent(const sf::Event& event)
{
	if (event.Type == sf::Event::KeyPressed)
	{
		if (event.Key.Code == sf::Key::Return)
		{
			zone_container_.Unload();
			zone_container_.Load(ZoneContainer::WORLD);
			next_map_name_ = ZoneContainer::WORLD;
			// respawn player
			player_ = new Player(sf::Vector2f(300, 300), app_.GetInput());
			zone_container_.GetActiveZone()->AddEntity(player_);
			SetMode(IN_GAME);
		}
	}
}


void Game::GameOverUpdate(float frametime)
{
	(void) frametime;
}


void Game::GameOverShow()
{
	app_.Clear();
	app_.Draw(message_);
}
