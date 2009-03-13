#include <iostream>

#include "MyWin.hpp"

using namespace gui;

MyWin::MyWin()
{
    // Chargement du descriptif XML
    Load("data/window/test.xml");

    mavar_ = 0;

    // Bind d'un int avec un label
    BindIntTo(ID_LABEL, &mavar_);
}

MyWin::~MyWin()
{

}

int MyWin::WindowCallback(const Control::ControlID id, const int p1, void *p2)
{
    std::cout << "MyWin callback [id:" << id << "]" << std::endl;

    mavar_ ++;
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
