#include <iostream>

#include "Label.hpp"
#include "../misc/MediaManager.hpp"

using namespace gui;
using namespace std;

Label::Label(const ControlPos& pos, const std::string& text) : Control(0, pos){

    text_.SetText(text);
    text_.SetFont(GET_FONT());
    text_.SetSize(12);
    text_.Move(pos.x, pos.y);
}

Label::~Label(){

}

void Label::Show(sf::RenderTarget& app){
    app.Draw(text_);
}
