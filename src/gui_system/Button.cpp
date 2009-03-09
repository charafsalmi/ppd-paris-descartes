#include <iostream>

#include "Button.hpp"
#include "../misc/MediaManager.hpp"

using namespace gui;
using namespace std;

Button::Button() : Control(){

}

Button::Button(ControlID id, const ControlPos& pos, const std::string& str) : Control(id, pos){
    img_ = GET_IMG(str.c_str());

    // Positionnement du contrôle
    rect_.Top = pos.y;
    rect_.Left = pos.x;
    rect_.Bottom = pos.y + img_.GetSize().y;
    rect_.Right = pos.x + img_.GetSize().x;
}

void Button::Render(sf::RenderTarget& app) const{
    #ifdef _DEBUG
    std::cout << "Show button" << std::endl;
    #endif

    app.Draw(img_);
}
