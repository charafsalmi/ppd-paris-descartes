#include <cstring>
#include <iostream>

#include "Game.hpp"
#include "SoundSystem.hpp"
#include "../misc/MediaManager.hpp"
#include "../misc/ConfigParser.hpp"
#include "../misc/Log.hpp"
#include "../gui/Splash.hpp"
#include "../gui/MiniMap.hpp"
#include "../entities/Player.hpp"
#include "../misc/LogDebug.hpp"
#include "../misc/LogConsole.hpp"
#include "../xml/tinyxml.h"

#define APP_WIDTH  Zone::WIDTH_PX
#define APP_HEIGHT (Zone::HEIGHT_PX + ControlPanel::HEIGHT_PX)
#define APP_BPP    32
#define APP_FPS    60
#define APP_TITLE  "PPD"
#define APP_STYLE  sf::Style::Titlebar | sf::Style::Close

#define CONFIG_FILE "config/config.css"


Game& Game::GetInstance()
{
	static Game self;
	return self;
}


Game::Game() :
	panel_(ControlPanel::GetInstance()),
	message_(GET_BITMAP_FONT("retro")),
	controller_(InputController::GetInstance())
{
	app_.Create(sf::VideoMode(APP_WIDTH, APP_HEIGHT, APP_BPP), APP_TITLE,APP_STYLE);
	app_.SetFramerateLimit(APP_FPS);

	const sf::Image& icon = GET_IMG("icon");
	app_.SetIcon(icon.GetWidth(), icon.GetHeight(), icon.GetPixelsPtr());

#ifndef NO_SPLASH
	Splash s(app_);
	s.Run();
#endif

#ifdef CONSOLE_TEST
	log_ = new LogConsole();
	Log::SetLogger(log_);
#else
	Log::SetLogger(new LogDebug());
#endif

	// default: panel on top
	zone_container_.SetPosition(0, ControlPanel::HEIGHT_PX);
	options_.panel_on_top = true;

	// load options
	ConfigParser config;
	if (config.LoadFromFile(CONFIG_FILE))
	{
		Output << "loading " << CONFIG_FILE << "..." << lEnd;

		config.SeekSection("Settings");
		config.ReadItem("panel_on_top", options_.panel_on_top);
		if (!options_.panel_on_top)
		{
			sf::Event event;
			event.Type = sf::Event::KeyPressed;
			event.Key.Code = sf::Key::PageDown; //dummy!
			InGameOnEvent(event, input::PANEL_DOWN);
		}

		config.ReadItem("verbosity", options_.verbosity);
		Log::SetVerboseLevel(options_.verbosity);
		Output << "verbose: " << options_.verbosity << lEnd;
	}
}


Game::~Game()
{
	// writing options
	ConfigParser config;
	config.SeekSection("Settings");
	config.WriteItem("panel_on_top", options_.panel_on_top ? 1 : 0);
	config.WriteItem("verbosity", options_.verbosity);

	config.SaveToFile(CONFIG_FILE);

	delete mini_map_;

	// delete player_; ?

	app_.Close();

#ifdef DUMB_MUSIC
	SetMusic(-1);
#endif

#ifdef CONSOLE_TEST
	delete log_;
#endif
}


void Game::Init()
{
	clock_.Reset();
	player_ = new Player(sf::Vector2f(300, 300));

	// chargement du conteneur de zones
	zone_container_.Load(ZoneContainer::WORLD);
	// chargement de la première zone
	zone_container_.SetActiveZone(0, 0, false);
	zone_container_.GetActiveZone()->AddEntity(player_);

	next_map_name_ = ZoneContainer::WORLD;

	// initialisation de la mini map
	mini_map_ = new MiniMap(zone_container_);
	// centrée dans la fenêtre
	mini_map_->SetPosition(
		(APP_WIDTH - mini_map_->GetWidth()) / 2,
		(APP_HEIGHT - mini_map_->GetHeight()) / 2);
	mini_map_->SetPlayerPosition(zone_container_.GetPlayerPosition());

	// InGame
	#ifdef START_ON_MENU
	SetMode(MAIN_MENU);
	#else
	SetMode(IN_GAME);
	#endif
}


int Game::Run()
{
	sf::Event event;
	input::Action action;
	float frametime;
	running_ = true;

	while (running_)
	{
		// POLLING
		while (app_.GetEvent(event))
		{
			// global events
			if (controller_.GetAction(event, action))
			{
				switch (action)
				{
					case input::EXIT_APP:
						running_ = false;
						break;
					case input::TAKE_SCREENSHOT:
						TakeScreenshot("screenshot");
						break;
					default:
						break;
				}
			}
			(this ->* on_event_meth_)(event, action);
		}

		// UPDATE
		frametime = app_.GetFrameTime();
		(this ->* update_meth_)(frametime);

		// RENDER
		(this ->* render_meth_)();
		app_.Display();
	}
	return 0;
}


void Game::TakeScreenshot(const char* directory)
{
	char currentTime[256];
	std::string filename;
	time_t t = time(NULL);

	strftime(currentTime, sizeof(currentTime), "%d-%m-%Y-%M%S", localtime(&t));

	filename = str_sprintf("%s/%s.png", directory, currentTime, t);

	panel_.PrintInfoText("capture d'ecran prise");
	app_.Capture().SaveToFile(filename);
}


void Game::ChangeZone(ZoneContainer::Direction direction)
{
	zone_container_.ChangeZone(direction);
	mini_map_->SetPlayerPosition(zone_container_.GetPlayerPosition());
}


void Game::ChangeZoneContainer(ZoneContainer::MapName map_name)
{
	Output << GAME_S << "Changement de map" << lEnd;

	// on retire le joueur de la zone de l'ancien conteneur
	Zone* active = zone_container_.GetActiveZone();
	active->RemoveEntity(player_);
	zone_container_.Unload();
	zone_container_.Load(map_name);
	if (!zone_container_.SetActiveZone(next_zone_cds_.x, next_zone_cds_.y, false))
	{
		OutputE << "Impossible d'activer la zone du conteneur cible" << lEnd;
		abort();
	}
	// insertion du joueur dans le nouveau conteneur
	active = zone_container_.GetActiveZone();
	active->AddEntity(player_);

	next_map_name_ = map_name;

	// on réinitialise la minimap
	delete mini_map_;
	mini_map_ = new MiniMap(zone_container_);
	mini_map_->SetPosition(
		(APP_WIDTH - mini_map_->GetWidth()) / 2,
		(APP_HEIGHT - mini_map_->GetHeight()) / 2);
	mini_map_->SetPlayerPosition(zone_container_.GetPlayerPosition());
}


void Game::Teleport(const Zone::Teleporter& tp)
{
	ZoneContainer::MapName map_name = (ZoneContainer::MapName) tp.zone_container;
	if (map_name != zone_container_.GetName())
	{
		Output << GAME_S << "Teleportation inter-conteneurs" << lEnd;
		next_map_name_ = map_name;
		next_zone_cds_ = tp.zone_coords;
	}
	else
	{
		Output << GAME_S << "Teleportation intra-conteneur" << lEnd;
		// on informe le conteneur que la zone active doit changer à la prochaine itération
		if (!zone_container_.SetActiveZone(tp.zone_coords.x, tp.zone_coords.y))
		{
			OutputE << GAME_S << "Mauvaise coordonnees pour SetActiveZone" << lEnd;
			abort();
		}
	}

	int x = tp.tile_coords.x * Tile::SIZE;
	int y = tp.tile_coords.y * Tile::SIZE;
	player_->SetPosition(x, y);
}


void Game::EndGame()
{
	message_.SetText(
			"Tu n'es pas le digne fils de Chuck Norris (Enter pour rejouer)");
	message_.SetPosition(10, 250);

	SetMode(GAME_OVER);
	SoundSystem::GetInstance().PlayMusic("game-over");
}


