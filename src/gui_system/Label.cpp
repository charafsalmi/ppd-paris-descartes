#include <iostream>

#include "Label.hpp"

using namespace gui;
using namespace std;

Label::Label(ControlPos pos, const std::string& text) : Control(0, pos){
    cout << "Creation d'un label " << text << endl;
}

Label::~Label(){

}

void Label::Show(sf::RenderTarget& app){

}
