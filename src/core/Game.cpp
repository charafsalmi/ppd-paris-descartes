#include "Game.hpp"
#include "SoundSystem.hpp"
#include "../misc/MediaManager.hpp"
#include "../misc/ConfigParser.hpp"
#include "../misc/Log.hpp"
#include "../misc/Die.hpp"
#include "../gui/Splash.hpp"
#include "../gui/MiniMap.hpp"
#include "../entities/Player.hpp"
#include "../misc/LogDebug.hpp"
#include "../misc/LogConsole.hpp"
#include "../xml/tinyxml.h"

#define APP_WIDTH  Zone::WIDTH_PX
#define APP_HEIGHT (Zone::HEIGHT_PX + ControlPanel::HEIGHT_PX)
#define APP_TITLE  "PPD"
#define APP_STYLE  (sf::Style::Titlebar | sf::Style::Close)
#define APP_BPP    32
#define APP_FPS    60

#define DEFAULT_VERBOSITY 5
#define CONFIG_FILE "config/config.txt"

#define MINIMAP_X 30
#define MINIMAP_Y 30

// emplacement par défaut si pas de sauvegarde
#define DEFAULT_MAP        "world"
#define DEFAULT_ZONE_X     0
#define DEFAULT_ZONE_Y     0
#define DEFAULT_PLAYER_POS sf::Vector2f(160, 256)


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
	Log::SetLogger(new LogDebug());
	LoadConfig(CONFIG_FILE);

	app_.Create(sf::VideoMode(APP_WIDTH, APP_HEIGHT, options_.bpp), APP_TITLE, APP_STYLE);
	app_.SetFramerateLimit(options_.fps);

	const sf::Image& icon = GET_IMG("icon");
	app_.SetIcon(icon.GetWidth(), icon.GetHeight(), icon.GetPixelsPtr());

#ifndef NO_SPLASH
	Splash splash(app_);
	if (!splash.Run())
	{
		exit(EXIT_SUCCESS);
	}
#endif

#ifdef CONSOLE_TEST
	log_ = new LogConsole();
	Log::SetLogger(log_);
#endif

	if (options_.panel_on_top)
	{
		map_.SetPosition(0, ControlPanel::HEIGHT_PX);
	}
    else
    {
		panel_.SetPosition(0, Zone::HEIGHT_PX);
    }

    option_win_ = new Option;
}


Game::~Game()
{
    SaveConfig(CONFIG_FILE);

    delete option_win_;
	delete mini_map_;
	app_.Close();
#ifdef CONSOLE_TEST
	delete log_;
#endif
	map_.Unload();
}


bool Game::LoadConfig(const std::string & str)
{
	// default options
	options_.bpp = APP_BPP;
	options_.fps = APP_FPS;
	options_.style = 0;
	options_.verbosity = DEFAULT_VERBOSITY;

	options_.panel_on_top = 1;

	ConfigParser config;
	Output << "loading " << CONFIG_FILE << "..." << lEnd;
	if (!config.LoadFromFile(str.c_str()))
	{
		OutputW << "utilisation de la configuration par défaut" << lEnd;
		return false;
	}

    // Engine options
    config.SeekSection("Engine");
    config.ReadItem("bpp", options_.bpp);
    config.ReadItem("style", options_.style);
    config.ReadItem("fps", options_.fps);
    config.ReadItem("verbosity", options_.verbosity);
    Log::SetVerboseLevel(options_.verbosity);

    Output << "verbose: " << options_.verbosity << lEnd;

	// Game options
    config.SeekSection("Settings");
    config.ReadItem("panel_on_top", options_.panel_on_top);
	bool music = false;
	if (config.ReadItem("enable_music", music) && !music)
	{
		SoundSystem::GetInstance().EnableMusic(false);
	}

	return true;
}


void Game::SaveConfig(const std::string& str) const
{
    // writing options
	ConfigParser config;
	config.SeekSection("Settings");
	config.WriteItem("panel_on_top", options_.panel_on_top ? 1 : 0);
	config.WriteItem("enable_music", SoundSystem::GetInstance().IsMusicEnabled() ? 1 : 0);

	config.SeekSection("Engine");
	config.WriteItem("style", options_.style);
	config.WriteItem("fps", options_.fps);
	config.WriteItem("bpp", options_.bpp);
	config.WriteItem("verbosity", options_.verbosity);

	config.SaveToFile(str.c_str());
}


void Game::SaveProgression(const char* filename) const
{
	ConfigParser parser;
	parser.SeekSection("Location");
	parser.WriteItem("map", map_.GetName());
	parser.WriteItem("zone_x", map_.GetPlayerPosition().x);
	parser.WriteItem("zone_y", map_.GetPlayerPosition().y);
	parser.WriteItem("x", (int) player_->GetPosition().x);
	parser.WriteItem("y", (int) player_->GetPosition().y);

	parser.SeekSection("Status");
	parser.WriteItem("hp", player_->GetHP());
	parser.WriteItem("money", player_->GetMoney());
	parser.WriteItem("frags", player_->GetFrags());

	parser.SeekSection("Inventory");
	const Inventory& inventory = player_->GetInventory();
	parser.WriteItem("item1", inventory.GetItem1Type());
	parser.WriteItem("item2", inventory.GetItem2Type());
	parser.WriteItem("item3", inventory.GetItem3Type());
	parser.WriteItem("stock", inventory.StockToString());
	// TODO sauver temps de jeu et minimap
	parser.SaveToFile(filename);
}


bool Game::LoadProgression(const char* filename)
{
	ConfigParser parser;

	if (parser.LoadFromFile(filename))
	{
		parser.SeekSection("Location");
		bool ok = true;
		std::string map_name;
		ok &= parser.ReadItem("map", map_name);
		int zx, zy;
		ok &= parser.ReadItem("zone_x", zx);
		ok &= parser.ReadItem("zone_y", zy);

		int x, y;
		ok &= parser.ReadItem("x", x);
		ok &= parser.ReadItem("y", y);

		parser.SeekSection("Status");
		int hp, money, frags;
		ok &= parser.ReadItem("frags", frags);
		ok &= parser.ReadItem("money", money);
		ok &= parser.ReadItem("hp", hp);

		parser.SeekSection("Inventory");
		int it1, it2, it3;
		ok &= parser.ReadItem("item1", it1);
		ok &= parser.ReadItem("item2", it2);
		ok &= parser.ReadItem("item3", it3);

		std::string stock;
		ok &= parser.ReadItem("stock", stock);

		if (ok)
		{
			map_.Load(map_name);
			map_.SetActiveZone(zx, zy, false);

			player_->SetPosition(x, y);
			player_->SetHP(hp);
			player_->AddGold(money);
			player_->AddFrag(frags);

			Inventory& inventory = player_->GetInventory();
			inventory.SetItem1Type(it1);
			inventory.SetItem2Type(it2);
			inventory.SetItem3Type(it3);
			inventory.StockFromString(stock);
			return true;
		}
		printf("fail at loading player save: %s\n", SAVE_FILE);
	}
	return false;
}


void Game::Init()
{
	clock_.Reset();
	player_ = new Player(sf::Vector2f(0, 0));

	if (!LoadProgression(SAVE_FILE))
	{
		// positionnement par défaut
		map_.Load(DEFAULT_MAP);
		map_.SetActiveZone(DEFAULT_ZONE_X, DEFAULT_ZONE_Y, false);
		player_->SetPosition(DEFAULT_PLAYER_POS);
	}
	need_map_update_ = false;
	map_name_ = "";
	map_.GetActiveZone()->AddEntity(player_);

	// initialisation de la mini map
	mini_map_ = new MiniMap(map_);
	mini_map_->SetPosition(
		map_.GetPosition().x + MINIMAP_X,
		map_.GetPosition().y + MINIMAP_Y);
	mini_map_->SetPlayerPosition(map_.GetPlayerPosition());
	#ifndef NO_START_MENU
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
			(this->*on_event_meth_)(event, action);
		}

		// UPDATE
		frametime = app_.GetFrameTime();
		(this->*update_meth_)(frametime);

		// RENDER
		(this->*render_meth_)();
		app_.Display();
	}
	return 0;
}


void Game::TakeScreenshot(const char* directory)
{
	char current_time[256];
	std::string filename;
	time_t t = time(NULL);

	strftime(current_time, sizeof current_time, "%d-%m-%Y-%M%S", localtime(&t));

	filename = str_sprintf("%s/%s.png", directory, current_time, t);

	panel_.PrintInfoText("capture d'ecran prise");
	app_.Capture().SaveToFile(filename);
}


void Game::ChangeZone(ZoneContainer::Direction direction)
{
	map_.ChangeZone(direction);
	mini_map_->SetPlayerPosition(map_.GetPlayerPosition());
}


void Game::ChangeMap(const std::string& map_name)
{
	// on retire le joueur de l'ancien carte
	Zone* active = map_.GetActiveZone();
	active->RemoveEntity(player_);
	map_.Unload();
	map_.Load(map_name);
	if (!map_.SetActiveZone(next_zone_cds_.x, next_zone_cds_.y, false))
	{
		DIE("mauvaises coordonnes pour la prochaine zone active (%d %d)",
				next_zone_cds_.x, next_zone_cds_.y);
	}
	// insertion du joueur dans la nouvelle carte
	active = map_.GetActiveZone();
	active->AddEntity(player_);

	// on réinitialise la minimap
	delete mini_map_;
	mini_map_ = new MiniMap(map_);
	mini_map_->SetPosition(
		map_.GetPosition().x + MINIMAP_X,
		map_.GetPosition().y + MINIMAP_Y);
	mini_map_->SetPlayerPosition(map_.GetPlayerPosition());
}


void Game::Teleport(const Zone::Teleporter& tp)
{
	if (tp.map_name != map_.GetName())
	{
		Output << GAME_S << "Teleportation inter-conteneurs" << lEnd;
		map_name_ = tp.map_name;
		need_map_update_ = true;
		next_zone_cds_ = tp.zone_coords;
	}
	else
	{
		Output << GAME_S << "Teleportation intra-conteneur" << lEnd;
		// on informe le conteneur que la zone active doit changer à la prochaine itération
		if (!map_.SetActiveZone(tp.zone_coords.x, tp.zone_coords.y))
		{
			DIE("mauvaises coordonnes pour la zone de teleportation (%d %d)",
				tp.zone_coords.x, tp.zone_coords.y);
		}
	}

	int x = tp.tile_coords.x * Tile::SIZE;
	int y = tp.tile_coords.y * Tile::SIZE;
	player_->SetPosition(x, y);
	mini_map_->SetPlayerPosition(map_.GetPlayerPosition());
}


void Game::EndGame()
{
	message_.SetText(
			"Tu n'es pas le digne fils de Chuck Norris (Enter pour rejouer)");
	message_.SetPosition(10, 250);

	SetMode(GAME_OVER);
	SoundSystem::GetInstance().PlayMusic("game-over");
}


