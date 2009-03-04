#include <cstring>
#include <iostream>

#include "Game.hpp"
#include "MediaManager.hpp"
#include "Splash.hpp"
#include "tinyxml/tinyxml.h"

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


Game::Game() :
	panel_(ControlPanel::GetInstance())
{
	app_.Create(sf::VideoMode(APP_WIDTH, APP_HEIGHT, APP_BPP), APP_TITLE);
	app_.SetFramerateLimit(APP_FPS);

	const sf::Image& icon = GET_IMG("icon");
	std::cerr << icon.GetWidth() << ", " << icon.GetHeight() << "\n";
	app_.SetIcon(icon.GetWidth(), icon.GetHeight(), icon.GetPixelsPtr());
	
	player_ = new Player(sf::Vector2f(300, 300), app_.GetInput());
	
	// chargement du conteneur de zones
	zone_container_.Load(ZoneContainer::WORLD, app_);
	// InGame
	SetMode(IN_GAME);
}


Game::~Game()
{
	app_.Close();

#ifdef DUMB_MUSIC
	SetMusic(-1);
#endif
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
		(this->*update_meth_)(frametime);
	
		// RENDER
		(this->*render_meth_)();
		app_.Display();
		
		if (zone_container_.GetName() != next_zone_name_)
		{
			ChangeZoneContainer(next_zone_name_);
		}
	}
}


void Game::ChangeZone(ZoneContainer::Direction direction)
{
	zone_container_.ChangeZone(direction);
}


void Game::ChangeZoneContainer(ZoneContainer::MapName zone_name)
{
	puts(" exécution de la demande de changement de conteneur");
	// on retire le joueur de la zone de l'ancien conteneur
	Zone* active = zone_container_.GetActiveZone();
	active->RemoveEntity(player_);
	zone_container_.Unload();
	zone_container_.Load(zone_name, app_);
	if (!zone_container_.SetActiveZone(next_zone_cds_.x, next_zone_cds_.y, false))
	{
		puts(" impossible d'activer la zone du conteneur cible");
		abort();
	}
	// insertion du joueur dans le nouveau conteneur
	active = zone_container_.GetActiveZone();
	active->AddEntity(player_);
	
	next_zone_name_ = zone_name;
}


void Game::Teleport(const Zone::Teleporter& tp)
{
	ZoneContainer::MapName zc_name = (ZoneContainer::MapName) tp.zone_container;
	if (zc_name != zone_container_.GetName())
	{
		puts(" [Game] téléportation inter-conteneurs");
		next_zone_name_ = zc_name;
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
	if (key == sf::Key::Return)
	{
		SetMode(INVENTORY);
	}
	player_->OnEvent(key);
}


void Game::InGameShow()
{
	zone_container_.Show(app_);
	panel_.Show(app_);
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
	zone_container_.Show(app_);
	panel_.GetInventory()->Show(app_);
}

