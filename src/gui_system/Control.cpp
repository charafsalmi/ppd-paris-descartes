#ifdef _DEBUG
#include <iostream>
#endif

#include "Control.hpp"

using namespace std;

Control::Control(){

}

Control::Control(ControlID id, ControlPos pos){
    id_ = id;
    position_ = pos;
}

Control::~Control(){

}
