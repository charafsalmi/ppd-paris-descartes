#include <iostream>

#include "MyWin.hpp"

using namespace gui;

MyWin::MyWin(){
    /* Chargement du descriptif XML */
    Load("data/window/test.xml");
}

int MyWin::WindowCallback(Control::ControlID id){
    switch(id){
        case IDEXIT:
            std::cout << "gui_system : IDEXIT called" << std::endl;
            return 1;
            break;
    }
    return 0;
}
