#include <iostream>

#include "Window.hpp"

#include "Button.hpp"
#include "Label.hpp"
#include "ProgressBar.hpp"

#include "../misc/MediaManager.hpp"
#include "../misc/Misc.hpp"

#include "../xml/tinyxml.h"

using namespace std;
using namespace gui;

Window::Window(){
    // 100*100
    rect_.Top = 0;
    rect_.Bottom = 100;
    rect_.Left = 100;
    rect_.Right = 0;
}

Window::Window(const Window& other){

}

Window::~Window(){
    UnLoad();
}

void Window::ManageEvent(const sf::Event& event){
    std::vector<Control*>::const_iterator it;

    for(it=controls_.begin();it!=controls_.end();it++){
        // Pour chaque contrôle on verifit si une action les concernes
        /*
        sf::Event::MouseButtonEvent
        Mouse::Button == Mouse::Left, ...
        int X, Y
        */
        /*if (p.Intersects((*it)->GetPosition())){
            cout << "Intersection !" << endl;
            // this->WindowCallback((*it)->GetID());
        }
        */

    }
    // this->WindowCallback(id);
}

void Window::Load(const std::string& xmlfile){
    background_ = GET_IMG("inventory-slot");
    background_.Resize(100, 100);

    // controls_.push_back(new Button(IDEXIT, Control::ControlPos(0,0), "button_prev"));
	/*
	TiXmlDocument doc;
    if (!doc.LoadFile(xmlfile.c_str()))
	{
		std::cerr << "can't open window definitions: " << xmlfile.c_str() << std::endl;
		abort();
	}
    TiXmlHandle handle(&doc);
	TiXmlElement* elem = handle.FirstChildElement().FirstChildElement().Element();

	const char* p = NULL;
	MediaManager& media = MediaManager::GetInstance();
	while (elem != NULL)
	{
		// id du control
		int id;
		if (elem->QueryIntAttribute("id", &id) != TIXML_SUCCESS)
		{
		    id = 0;
		}

		// Image à charger (si il y a lieu)
		p = elem->Attribute("pic");
		if(p == NULL){
		    cerr << "Pic attribute not defined" << endl;
		    continue;
		}

		// Position du control
		int x,y;
		if (elem->QueryIntAttribute("x", &x) != TIXML_SUCCESS){
			x = 0;
		}
		if (elem->QueryIntAttribute("y", &y) != TIXML_SUCCESS){
			y = 0;
		}

        controls_.push_back(new Button(id, Control::ControlPos(x,y), p));

		elem = elem->NextSiblingElement();
	}
	*/
}

void Window::UnLoad(){
    /* Access violation
    std::vector<Control*>::const_iterator it;
    for(it=controls_.begin();it!=controls_.end();it++){
        delete (*it);
        //it.remove();
    }
    */
}

void Window::Show(sf::RenderTarget& app){
    std::vector<Control*>::const_iterator it;

    app.Draw(background_);
    for(it=controls_.begin();it!=controls_.end();it++){
        (*it)->Show(app);
    }
}
