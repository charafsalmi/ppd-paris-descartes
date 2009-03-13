#include <iostream>

#include "../misc/Log.hpp"
#include "Label.hpp"
#include "../misc/MediaManager.hpp"

using namespace gui;
using namespace std;

Label::Label(const ControlID id, const ControlPos& pos, const std::string& text) :
	Control(id, pos),
	text_(GET_BITMAP_FONT("mono12-white"))
{
    ptr_ = NULL;
	text_.SetText(text);
}

Label::~Label(){

}

void Label::Render(sf::RenderTarget& app) const{
    app.Draw(text_);
}

void Label::LinkInt(int *ptr){
    cout << "test" << endl;
    ptr_ = ptr;
    mode_ = LINKED;
}
