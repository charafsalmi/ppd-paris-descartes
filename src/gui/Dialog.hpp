#ifndef DIALOG_HPP
#define DIALOG_HPP

#include "../gui_system/Window.hpp"

class Dialog : public gui::Window{
public:
    Dialog();

private:
    int WindowCallback(const gui::Control::ControlID id, const int p1, void *p2);
};

#endif
