#include <iostream>

#include "Label.hpp"

using namespace gui;
using namespace std;

Label::Label(ControlPos pos, const std::string& text) : Control(0, pos){
    if(!font_.LoadFromFile("data/font/VeraMono.ttf")){
        cerr << "Impossible de charger la font" << endl;
    }
    text_.SetText(text);
    text_.SetFont(font_);
    text_.SetSize(12);

    text_.Move(pos.x, pos.y);
}

Label::~Label(){

}

void Label::Show(sf::RenderTarget& app){
    app.Draw(text_);
}
