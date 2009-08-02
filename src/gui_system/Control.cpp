#include "Control.hpp"
#include "../misc/Log.hpp"

using namespace std;
using namespace gui;

Control::Control(ControlID id, const ControlPos& pos, const string & s)
{
    curr_state_ = NORMAL;
    accepted_states_ = NORMAL;

    text_ = s;

    ptr_ = NULL;
    mode_ = PLAIN;

	SetPosition(pos.x, pos.y);
    id_ = id;
    // rect_ = pos;
}

Control::~Control(){

}

void Control::LinkInt(int *ptr)
{
    Log::W() << "LinkInt" << id_ << "\n";
    mode_ = LINKED_INT;
    ptr_ = ptr;
}

void Control::LinkChar(char *ptr)
{
    Log::W() << "LinkChar" << id_ << "\n";
    mode_ = LINKED_CHAR;
    ptr_ = ptr;
}

void Control::LinkFloat(float* ptr)
{
	Log::W() << "LinkFloat" << id_ <<"\n";
	mode_ = LINKED_FLOAT;
	ptr_ = ptr;
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
