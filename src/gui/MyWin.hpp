#ifndef MYWIN_HPP
#define MYWIN_HPP

#include "../gui_system/Window.hpp"

class MyWin : public gui::Window{
public:
    MyWin();
    ~MyWin();

private:
    /// Listing des fenêtres
    enum Windows{
        ID_LABEL=105
    };

    int mavar_;
    int WindowCallback(const gui::Control::ControlID id, const int p1, void *p2);
};

#endif
