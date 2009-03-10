#ifndef WINPAUSE_HPP
#define WINPAUSE_HPP

#include "../gui_system/Window.hpp"

class WinPause : public gui::Window{
public:
    WinPause();

    /** Retours de l'inventaire */
    enum
    {
        _EXIT=101
    };

private:
    int WindowCallback(const gui::Control::ControlID id, const int p1, void *p2);
};

#endif
