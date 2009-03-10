#ifndef MYWIN_HPP
#define MYWIN_HPP

#include "../gui_system/Window.hpp"

class WinPause : public gui::Window{
public:
    WinPause();

private:
    int WindowCallback(const gui::Control::ControlID id, const int p1, void *p2);
};

#endif
