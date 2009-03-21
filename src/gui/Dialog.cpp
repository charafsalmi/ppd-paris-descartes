#include "Dialog.hpp"

using namespace gui;

Dialog::Dialog()
{
    Load("data/window/dialog.xml");
}

int Dialog::WindowCallback(const Control::ControlID id, const int p1, void *p2)
{

    return 0;
}
