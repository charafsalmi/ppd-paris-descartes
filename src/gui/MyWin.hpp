#ifndef MYWIN_HPP
#define MYWIN_HPP

#include "../gui_system/Window.hpp"

class MyWin : public gui::Window{
public:
    MyWin();

    int WindowCallback(gui::Control::ControlID id); // private?
};

#endif
