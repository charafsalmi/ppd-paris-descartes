#include <iostream>

#include "Window.hpp"

#include "Button.hpp"
#include "Label.hpp"
#include "ProgressBar.hpp"

#include "../misc/MediaManager.hpp"
#include "../misc/Misc.hpp"

#include "../xml/tinyxml.h"

using namespace std;

Window::Window(){
    position_ = sf::Vector2f(0, 0);
}

Window::Window(const Window& other){

}

Window::~Window(){
    std::vector<Control*>::const_iterator it;
    for(it=controls_.begin();it!=controls_.end();it++){
        delete (*it);
    }
}

void Window::ManageEvent(const sf::Event& event){
}

void Window::Load(const std::string& xmlfile){
    background_ = GET_IMG("inventory-slot");
    background_.Resize(100, 100);

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
}

void Window::Show(sf::RenderTarget& app){
    std::vector<Control*>::const_iterator it;

    app.Draw(background_);
    for(it=controls_.begin();it!=controls_.end();it++){
        (*it)->Show(app);
    }
}
