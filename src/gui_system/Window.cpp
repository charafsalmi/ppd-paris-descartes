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
        sf::Event::MouseButtonEvent, event.MouseButtonEvent.X
        Mouse::Button == Mouse::Left, ...
        int X, Y
        */
        /*if ((*it)->IsClicked(event.))){
            cout << "Intersection !" << endl;
            // this->WindowCallback((*it)->GetID());
        }
        */

    }
    // this->WindowCallback(id);
}

void Window::Load(const std::string& xmlfile){
    int id, x, y, w, h;
    const char* p = NULL;

    TiXmlDocument doc;
    TiXmlElement* controls_elem, *elem;
    TiXmlNode *node;

    if (!doc.LoadFile(xmlfile.c_str())){
		std::cerr << "can't open window definitions: " << xmlfile << std::endl;
		abort();
	}

    node = doc.FirstChild("window");
    elem = node->ToElement();
    if(elem != NULL){
        if (elem->QueryIntAttribute("x", &x) != TIXML_SUCCESS){
		    cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
		}
        if (elem->QueryIntAttribute("y", &y) != TIXML_SUCCESS){
		    cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
		}
        if (elem->QueryIntAttribute("w", &w) != TIXML_SUCCESS){
		    cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
		}
        if (elem->QueryIntAttribute("h", &h) != TIXML_SUCCESS){
		    cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
		}
        p = elem->Attribute("background");
        if(p == NULL){
            cout << "Warning: No window's background" << endl;
        }

        background_ = GET_IMG(p);
        background_.Resize(w, h);

        rect_.Top = x;
        rect_.Bottom = h;
        rect_.Left = w;
        rect_.Right = y;
    }

	node = doc.FirstChild("controls")->FirstChildElement();
	controls_elem = node->ToElement();
	while (controls_elem != NULL){
	    // Position du widget (nécéssaire à tout les contrôles)
		if (controls_elem->QueryIntAttribute("x", &x) != TIXML_SUCCESS){
		    cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
			x = 0;
			break;
		}
		if (controls_elem->QueryIntAttribute("y", &y) != TIXML_SUCCESS){
		    cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
			y = 0;
			break;
		}

	    std::string s1 = controls_elem->Value();

	    if(s1 == "button"){
            // id du widget
            if(controls_elem->QueryIntAttribute("id", &id) != TIXML_SUCCESS){
                cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
                id = 0;
            }

            // Image à charger
            p = controls_elem->Attribute("pic");
            if(p == NULL){
                cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
                break;
            }

            controls_.push_back(new Button(id, Control::ControlPos(x,y), p));
	    }else if(s1 == "label"){
	        controls_.push_back(new Label(Control::ControlPos(x,y), controls_elem->GetText()));
	    }

		controls_elem = controls_elem->NextSiblingElement();
	}
}

void Window::UnLoad(){
    std::vector<Control*>::const_iterator it;
    for(it=controls_.begin();it!=controls_.end();it++){
        delete (*it);
    }
    controls_.clear();
}

void Window::Show(sf::RenderTarget& app){
    std::vector<Control*>::const_iterator it;

    app.Draw(background_);
    for(it=controls_.begin();it!=controls_.end();it++){
        (*it)->Show(app);
    }
}
