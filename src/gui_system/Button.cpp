#ifdef _DEBUG
#include <iostream>
#endif

#include "Button.hpp"
#include "../misc/MediaManager.hpp"

Button::Button() : Control(){

}

Button::Button(ControlID id, ControlPos pos, const std::string& str) : Control(id, pos){
    img_ = GET_IMG(str.c_str());
}

Button::~Button(){

}

void Button::Show(sf::RenderTarget& app){
    #ifdef _DEBUG
    std::cout << "Show button" << std::endl;
    #endif

    app.Draw(img_);
}
