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
	SetMode(IN_GAME);
	SoundSystem::GetInstance().PlayMusic("ingame");
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


void Game::SetMode(Mode mode)
{
	// initialisation des callbacks
	switch (mode)
	{
		case IN_GAME:
			Output << "Passage en mode INGAME" << lEnd;

			on_event_meth_ = &Game::InGameOnEvent;
			update_meth_ = &Game::DefaultUpdate;
			render_meth_ = &Game::InGameShow;
			player_->Unlock();
			break;
		case INVENTORY:
			Output << "Passage en mode INVENTAIRE" << lEnd;

			on_event_meth_ = &Game::InventoryOnEvent;
			update_meth_ = &Game::DefaultUpdate;
			render_meth_ = &Game::InventoryShow;
			player_->Lock();
			break;
		case GAME_OVER:
			Output << "Passage en mode GAMEOVER" << lEnd;

			on_event_meth_ = &Game::GameOverOnEvent;
			update_meth_ = &Game::GameOverUpdate;
			render_meth_ = &Game::GameOverShow;
			break;
		case PAUSE:
			Output << "Passage en mode PAUSE" << lEnd;

			on_event_meth_ = &Game::PauseOnEvent;
			update_meth_ = &Game::PauseUpdate;
			render_meth_ = &Game::PauseShow;
			break;
		case MINI_MAP:
			Output << "Passage en mode MINIMAP" << lEnd;

			on_event_meth_ = &Game::MiniMapOnEvent;
			update_meth_ = &Game::DefaultUpdate;
			render_meth_ = &Game::MiniMapShow;
			player_->Lock();
			break;
	}
}

// méthodes callbacks

void Game::DefaultUpdate(float frametime) {
	panel_.Update(frametime);
	zone_container_.Update(frametime);
}

// IN_GAME

void Game::InGameOnEvent(const sf::Event& event, input::Action action)
{
	sf::Key::Code key = event.Key.Code;
#ifdef WINDOW_TEST
	fen_.ManageEvent(event);
#endif
	if (zone_container_.Scrolling())
	{
		return;
	}

	switch (action)
	{
		case input::SHOW_INVENTORY:
			SetMode(INVENTORY);
			break;
		case input::PANEL_UP:
			panel_.SetPosition(0, 0);
			zone_container_.SetPosition(0, ControlPanel::HEIGHT_PX);
			options_.panel_on_top = true;
			break;
		case input::PANEL_DOWN:
			panel_.SetPosition(0, Zone::HEIGHT_PX);
			zone_container_.SetPosition(0, 0);
			options_.panel_on_top = false;
			break;
		case input::PAUSE:
			SetMode(PAUSE);
			break;
		case input::SHOW_MINIMAP:
			SetMode(MINI_MAP);
			break;
		default:
			player_->OnEvent(action);
			break;
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
#ifdef CONSOLE_TEST
	app_.Draw(*log_);
#endif

#ifdef WINDOW_TEST
	app_.Draw(fen_);
#endif
}

// INVENTORY

void Game::InventoryOnEvent(const sf::Event& event, input::Action action)
{
	// TODO: Evenement à déporter dans le gestionnaire de fenêtre
	if (action == input::SHOW_INVENTORY)
	{
		SetMode(IN_GAME);
	}
	if (panel_.GetInventory()->ManageEvent(event) == WinInventory::_CLOSE)
	{
		SetMode(IN_GAME);
	}
	panel_.GetInventory()->OnEvent(action);
}


void Game::InventoryShow()
{
	app_.Draw(zone_container_);
	app_.Draw(panel_);
	app_.Draw(*panel_.GetInventory());
}

// PAUSE

void Game::PauseOnEvent(const sf::Event& event, input::Action action)
{
	if (action == input::PAUSE)
	{
		SetMode(IN_GAME);
	}
	if (pause_.ManageEvent(event) == WinPause::_EXIT)
	{
		running_ = false;
	}
}


void Game::PauseUpdate(float)
{
}


void Game::PauseShow()
{
	app_.Draw(pause_);
}

// GAME_OVER

void Game::GameOverOnEvent(const sf::Event& event, input::Action action)
{
	if (event.Type == sf::Event::KeyPressed)
	{
		if (event.Key.Code == sf::Key::Return)
		{
			zone_container_.Unload();
			Init();
		}
	}
}


void Game::GameOverUpdate(float)
{
}


void Game::GameOverShow()
{
	app_.Clear();
	app_.Draw(message_);
}

// MINI_MAP

void Game::MiniMapOnEvent(const sf::Event& event, input::Action action)
{
	if (action == input::SHOW_MINIMAP)
	{
		SetMode(IN_GAME);
	}
}


void Game::MiniMapShow()
{
	app_.Draw(zone_container_);
	app_.Draw(panel_);
	app_.Draw(*mini_map_);
}
