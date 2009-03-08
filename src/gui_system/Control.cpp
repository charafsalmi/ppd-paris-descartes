#ifdef _DEBUG
#include <iostream>
#endif

#include "Control.hpp"

using namespace std;
using namespace gui;

Control::Control(){

}

Control::Control(ControlID id, ControlPos pos){
    id_ = id;
    // rect_ = pos;
}

Control::~Control(){

}
