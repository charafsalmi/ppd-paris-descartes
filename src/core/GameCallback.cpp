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
            // HACK: Pour lancer la musique du menu au menu
            SoundSystem::GetInstance().PlayMusic("title");

            Output << "Passage en mode MAIN_MENU" << lEnd;

            on_event_meth_ = &Game::MainMenuOnEvent;
			update_meth_ = &Game::MainMenuUpdate;
			render_meth_ = &Game::MainMenuShow;
            break;
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
        case OPTION:
            Output << "Passage en mode OPTION" << lEnd;

            on_event_meth_ = &Game::OptionOnEvent;
			update_meth_ = &Game::OptionUpdate;
			render_meth_ = &Game::OptionShow;
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

// IN_GAME /////////////////////////////////////////////////////////

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

// INVENTORY /////////////////////////////////////////////////////////

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

// PAUSE /////////////////////////////////////////////////////////

void Game::PauseOnEvent(const sf::Event& event, input::Action action)
{
    int x = pause_.ManageEvent(event);

	if (action == input::PAUSE)
	{
		SetMode(IN_GAME);
		return;
	}

	switch(x){
	    case WinPause::_EXIT:
            running_ = false;
            break;
        case WinPause::_OPTION:
            SetMode(OPTION);
            break;
	}
}


void Game::PauseUpdate(float)
{
}

void Game::PauseShow()
{
    app_.Draw(zone_container_);
	app_.Draw(panel_);

	app_.Draw(pause_);
}

// GAME_OVER /////////////////////////////////////////////////////////

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

// MINI_MAP /////////////////////////////////////////////////////////

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

// MAIN_MENU /////////////////////////////////////////////////////////

void Game::MainMenuOnEvent(const sf::Event& event, input::Action action)
{
	int x = mmenu_.ManageEvent(event);

	switch(x){
	    case MainMenu::START_GAME:
            OutputD << "Lancement du jeu !" << lEnd;
            SetMode(IN_GAME);

            // HACK: Pour lancer la musique du jeu
            SoundSystem::GetInstance().PlayMusic("default");
            break;
        case MainMenu::EXIT_GAME:
            OutputD << "On quitte le programme ..." << lEnd;
            SetMode(IN_GAME);
            running_ = false;
            break;
	}
}

void Game::MainMenuUpdate(float frametime)
{

}

void Game::MainMenuShow()
{
	app_.Draw(mmenu_);
}

// OPTION /////////////////////////////////////////////////

void Game::OptionOnEvent(const sf::Event& event, input::Action action)
{
	int x = option_win_->ManageEvent(event);

	switch(x){
	    case Option::_EXIT:
            SetMode(PAUSE);
            break;
	}
}

void Game::OptionUpdate(float frametime)
{

}

void Game::OptionShow()
{
    app_.Draw(zone_container_);
	app_.Draw(panel_);
	app_.Draw(*option_win_);
}
