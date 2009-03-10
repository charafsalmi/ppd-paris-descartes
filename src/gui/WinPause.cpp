#include <iostream>

#include "WinPause.hpp"

using namespace gui;

WinPause::WinPause(){
    /* Chargement du descriptif XML */
    Load("data/window/pause.xml");
}

int WinPause::WindowCallback(const Control::ControlID id, const int p1, void *p2){
    switch(id){
        case IDEXIT:
            std::cout << "gui_system : IDEXIT called" << std::endl;
            return 1;
            break;
    }
    return 0;
}
