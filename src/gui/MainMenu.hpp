#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "../gui_system/Window.hpp"

class MainMenu: public gui::Window
{
public:
    MainMenu();

    enum{
        START_GAME=1000,
        EXIT_GAME
    };
private:
    /// Liste des composants de la fenêtre
    enum Windows
    {
        ID_OPTION=7000
    };

    int WindowCallback(gui::Control::ControlID id);
};

#endif
