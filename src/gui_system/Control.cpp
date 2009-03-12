#ifdef _DEBUG
#include <iostream>
#endif

#include "Control.hpp"

using namespace std;
using namespace gui;

Control::Control(ControlID id, const ControlPos& pos){
    curr_state_ = NORMAL;
    accepted_states_ = NORMAL;

	SetPosition(pos.x, pos.y);
    id_ = id;
    // rect_ = pos;
}

Control::~Control(){

}


void Control::ChangeSprite(const sf::Sprite& nming)
{
	(void) nming;
}


void Control::OnTextEntered(sf::Uint32 unicode)
{
	(void) unicode;
}


void Control::OnKeyPressed(sf::Key::Code key)
{
	(void) key;
}
