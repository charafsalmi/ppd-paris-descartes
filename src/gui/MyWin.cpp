#include <iostream>

#include "MyWin.hpp"

using namespace gui;

MyWin::MyWin()
{
    /* Chargement du descriptif XML */
    Load("data/window/test.xml");
}

MyWin::~MyWin()
{

}

int MyWin::WindowCallback(const Control::ControlID id, const int p1, void *p2)
{
    /*
    switch(id){
        case IDEXIT:
            std::cout << "gui_system : IDEXIT called" << std::endl;
            return 1;
            break;
    }
    */
    return 0;
}
