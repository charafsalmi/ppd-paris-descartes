#ifndef WINPAUSE_HPP
#define WINPAUSE_HPP

#include "../gui_system/Window.hpp"

class WinPause: public gui::Window
{
public:
    WinPause();

    enum
    {
        _OPTION=101, _EXIT
    };

private:
    /** Liste des fenêtres */
    enum
    {
        ID_OPTION=7001
    };

    int WindowCallback(gui::Control::ControlID id);
};

#endif
