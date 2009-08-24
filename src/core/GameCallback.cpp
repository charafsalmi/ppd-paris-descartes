#include "Game.hpp"
#include "SoundSystem.hpp"
#include "../misc/Log.hpp"
#include "../gui/Splash.hpp"
#include "../gui/MiniMap.hpp"
#include "../entities/Player.hpp"


void Game::SetMode(Mode mode)
{
	// initialisation des callbacks
	switch (mode)
	{
	    case MAIN_MENU:
			Output << "Passage en mode MAIN_MENU" << lEnd;
			SoundSystem::GetInstance().PlayMusic("title");

			on_event_meth_ = &Game::MainMenuOnEvent;
			update_meth_ = &Game::NoUpdate;
			render_meth_ = &Game::MainMenuShow;
			break;
		case IN_GAME:
			Output << "Passage en mode INGAME" << lEnd;
			player_->Unlock();

			on_event_meth_ = &Game::InGameOnEvent;
			update_meth_ = &Game::DefaultUpdate;
			render_meth_ = &Game::InGameShow;
			break;
		case INVENTORY:
			Output << "Passage en mode INVENTAIRE" << lEnd;
			player_->Lock();

			on_event_meth_ = &Game::InventoryOnEvent;
			update_meth_ = &Game::DefaultUpdate;
			render_meth_ = &Game::InventoryShow;
			break;
		case GAME_OVER:
			Output << "Passage en mode GAMEOVER" << lEnd;

			on_event_meth_ = &Game::GameOverOnEvent;
			update_meth_ = &Game::NoUpdate;
			render_meth_ = &Game::GameOverShow;
			break;
		case PAUSE:
			Output << "Passage en mode PAUSE" << lEnd;

			on_event_meth_ = &Game::PauseOnEvent;
			update_meth_ = &Game::NoUpdate;
			render_meth_ = &Game::PauseShow;
			break;
		case OPTION:
			Output << "Passage en mode OPTION" << lEnd;

			on_event_meth_ = &Game::OptionOnEvent;
			update_meth_ = &Game::NoUpdate;
			render_meth_ = &Game::OptionShow;
			break;
		case MINI_MAP:
			Output << "Passage en mode MINIMAP" << lEnd;

			on_event_meth_ = &Game::InGameOnEvent;
			update_meth_ = &Game::DefaultUpdate;
			render_meth_ = &Game::MiniMapShow;
			break;
	}
	mode_ = mode;
}

// méthodes callbacks

void Game::DefaultUpdate(float frametime)
{
	panel_.Update(frametime);
	map_.Update(frametime);
}


void Game::NoUpdate(float)
{
}

// IN_GAME /////////////////////////////////////////////////////////

void Game::InGameOnEvent(const sf::Event&, input::Action action)
{
	if (map_.Scrolling())
	{
		return;
	}

	switch (action)
	{
		case input::SHOW_INVENTORY:
			SetMode(INVENTORY);
			break;
		case input::SAVE_PROGRESSION:
			SaveProgression(SAVE_FILE);
			panel_.PrintInfoText("Partie enregistree !");
			break;
		case input::PANEL_UP:
			panel_.SetPosition(0, 0);
			map_.SetPosition(0, ControlPanel::HEIGHT_PX);
			options_.panel_on_top = true;
			break;
		case input::PANEL_DOWN:
			panel_.SetPosition(0, Zone::HEIGHT_PX);
			map_.SetPosition(0, 0);
			options_.panel_on_top = false;
			break;
		case input::PAUSE:
			SetMode(PAUSE);
			break;
		case input::SHOW_MINIMAP:
			SetMode(mode_ == MINI_MAP ? IN_GAME : MINI_MAP);
			break;
		default:
			player_->OnEvent(action);
			break;
	}
}


void Game::InGameShow()
{
	app_.Draw(map_);
	app_.Draw(panel_);

	if (need_map_update_)
	{
		ChangeMap(map_name_);
		need_map_update_ = false;
	}
#ifdef CONSOLE_TEST
	app_.Draw(*log_);
#endif
}

// INVENTORY /////////////////////////////////////////////////////////

void Game::InventoryOnEvent(const sf::Event& event, input::Action action)
{
	if (action == input::SHOW_INVENTORY)
	{
		SetMode(IN_GAME);
	}
	player_->GetInventory().OnEvent(action);
}


void Game::InventoryShow()
{
	app_.Draw(map_);
	app_.Draw(panel_);
	app_.Draw(player_->GetInventory());
}

// PAUSE /////////////////////////////////////////////////////////

void Game::PauseOnEvent(const sf::Event& event, input::Action action)
{
	if (action == input::PAUSE)
	{
		SetMode(IN_GAME);
	}
	else
	{
		switch (pause_.ManageEvent(event))
		{
			case WinPause::_EXIT:
				running_ = false;
				break;
			case WinPause::_OPTION:
				SetMode(OPTION);
				break;
		}
	}
}


void Game::PauseShow()
{
	app_.Draw(map_);
	app_.Draw(panel_);
	app_.Draw(pause_);
}

// GAME_OVER /////////////////////////////////////////////////////////

void Game::GameOverOnEvent(const sf::Event& event, input::Action)
{
	if (event.Type == sf::Event::KeyPressed)
	{
		if (event.Key.Code == sf::Key::Return)
		{
			map_.Unload();
			Init();
		}
	}
}


void Game::GameOverShow()
{
	app_.Clear();
	app_.Draw(message_);
}

// MINI_MAP /////////////////////////////////////////////////////////

void Game::MiniMapShow()
{
	InGameShow();
	mini_map_->Update(app_.GetFrameTime());
	app_.Draw(*mini_map_);
}

// MAIN_MENU /////////////////////////////////////////////////////////

void Game::MainMenuOnEvent(const sf::Event& event, input::Action)
{
	switch (mmenu_.ManageEvent(event))
	{
		case MainMenu::START_GAME:
			OutputD << "Lancement du jeu !" << lEnd;
			SetMode(IN_GAME);

			SoundSystem::GetInstance().PlayMusic(map_.GetActiveZone()->GetMusicName());
			break;
		case MainMenu::EXIT_GAME:
			OutputD << "On quitte le programme ..." << lEnd;
			SetMode(IN_GAME);
			running_ = false;
			break;
	}
}


void Game::MainMenuShow()
{
	app_.Draw(mmenu_);
}

// OPTION /////////////////////////////////////////////////

void Game::OptionOnEvent(const sf::Event& event, input::Action)
{
	switch (option_win_->ManageEvent(event))
	{
		case Option::_EXIT:
			SetMode(PAUSE);
			break;
	}
}


void Game::OptionShow()
{
	app_.Draw(map_);
	app_.Draw(panel_);
	app_.Draw(*option_win_);
}
