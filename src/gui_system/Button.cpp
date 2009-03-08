#include <iostream>

#include "Button.hpp"
#include "../misc/MediaManager.hpp"

using namespace gui;
using namespace std;

Button::Button() : Control(){

}

Button::Button(ControlID id, ControlPos pos, const std::string& str) : Control(id, pos){
    img_ = GET_IMG(str.c_str());
    //ControlPos p = img_.GetSize();

    cout << "Image size : " << img_.GetSize().x;

    // Positionnement du contrôle
    /*rect_.Top = pos.y;
    rect_.Left = pos.x;
    rect_.Bottom = pos.y + p.y;
    rect_.Right = pos.x + p.x;
    */
}

void Button::Show(sf::RenderTarget& app){
    #ifdef _DEBUG
    std::cout << "Show button" << std::endl;
    #endif

    app.Draw(img_);
}
