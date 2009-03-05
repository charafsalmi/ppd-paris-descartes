#ifdef DEBUG
#include <iostream>
#endif

#include "Window.hpp"

#include "../misc/MediaManager.hpp"
#include "../misc/Misc.hpp"

Window::Window(){
    position_ = sf::Vector2f(0, 0);
}

Window::Window(const Window& other){

}

Window::~Window(){

}

void Window::ManageEvent(sf::Event event){
}

void Window::Load(const std::string& xmlfile){
    background_ = GET_IMG("inventory-slot");
    background_.Resize(100, 100);
}

void Window::Show(sf::RenderTarget& app){
    app.Draw(background_);
}
