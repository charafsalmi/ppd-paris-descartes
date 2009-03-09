#ifdef _DEBUG
#include <iostream>
#endif

#include "Control.hpp"

using namespace std;
using namespace gui;

Control::Control(){

}

Control::Control(ControlID id, const ControlPos& pos){
	SetPosition(pos.x, pos.y);
    id_ = id;
    // rect_ = pos;
}

Control::~Control(){

}
