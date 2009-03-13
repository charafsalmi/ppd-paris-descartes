#include "MyWin.hpp"
#include "../misc/Log.hpp"

using namespace gui;

MyWin::MyWin()
{
    // Chargement du descriptif XML
    Load("data/window/test.xml");

    mavar_ = 0;

    c_ = new char[256];
    strcpy(c_, "dynamic text");

    // Bind d'un int avec un label
    BindIntTo(ID_PROGRESS, &mavar_);
    BindCharTo(ID_LABEL2, c_);
}

MyWin::~MyWin()
{
    delete[] c_;
}

int MyWin::WindowCallback(const Control::ControlID id, const int p1, void *p2)
{
    Output << "MyWin callback [id:" << id << "]";

    mavar_ ++;

    strcpy(c_, "other text");
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
