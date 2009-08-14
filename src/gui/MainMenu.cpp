#include "MainMenu.hpp"

using namespace gui;

MainMenu::MainMenu()
{
    Load("data/window/main_menu.xml");
}

int MainMenu::WindowCallback(gui::Control::ControlID id)
{
    switch (id)
    {
        case IDEXIT:
            return START_GAME;
        case IDCONFIRM:
            return EXIT_GAME;
    }
    return 0;
}
