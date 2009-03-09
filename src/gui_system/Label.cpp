#include <iostream>

#include "Label.hpp"
#include "../misc/MediaManager.hpp"

using namespace gui;
using namespace std;

Label::Label(const ControlID id, const ControlPos& pos, const std::string& text) : Control(id, pos){

    text_.SetText(text);
    text_.SetFont(GET_FONT());
    text_.SetSize(12);
}

Label::~Label(){

}

void Label::Render(sf::RenderTarget& app) const{
    app.Draw(text_);
}
