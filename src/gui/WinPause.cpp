#include "WinPause.hpp"

using namespace gui;

WinPause::WinPause(){
    // Chargement du descriptif XML
    Load("data/window/pause.xml");
}

int WinPause::WindowCallback(Control::ControlID id)
{
    switch(id)
    {
        case IDEXIT:
            return _EXIT;
        case ID_OPTION:
            return _OPTION;
    }
    return 0;
}
