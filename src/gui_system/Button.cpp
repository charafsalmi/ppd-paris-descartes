#include <iostream>

#include "Button.hpp"
#include "../misc/MediaManager.hpp"

using namespace gui;
using namespace std;

Button::Button() : Control(){

}

Button::Button(ControlID id, const ControlPos& pos, const std::string& str) : Control(id, pos){
    img_ = GET_IMG(str.c_str());

    img_.SetPosition(pos.x, pos.y);

    // Positionnement du contrôle
    rect_.Top = (int)pos.y;
    rect_.Left = (int)pos.x;
    rect_.Bottom = (int)pos.y + img_.GetSize().y;
    rect_.Right = (int)pos.x + img_.GetSize().x;
}

void Button::Show(sf::RenderTarget& app){
    #ifdef _DEBUG
    std::cout << "Show button" << std::endl;
    #endif

    app.Draw(img_);
}
