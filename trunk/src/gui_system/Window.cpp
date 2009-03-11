#include <iostream>

#include "Window.hpp"

#include "Button.hpp"
#include "Label.hpp"
#include "ProgressBar.hpp"
#include "TextBox.hpp"

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

Control *Window::GetFromID(Control::ControlID id){
    std::vector<Control*>::const_iterator it;
    for(it=controls_.begin();it!=controls_.end();it++){
        if((*it)->GetID() == id){
            return (*it);
        }
    }
    return NULL;
}

int Window::ManageEvent(const sf::Event& event){
    std::vector<Control*>::const_iterator it;

    int r;

    if(event.Type == sf::Event::MouseMoved){
        // transormations des coords absolues en coords relatives
        int x = event.MouseMove.X - GetPosition().x;
        int y = event.MouseMove.Y - GetPosition().y;

        //if(event.Type == sf::Event::MouseMoved){
        for (it=controls_.begin();it!=controls_.end();it++){
            // Pour chaque widget on verifit si une action les concerne
            if((*it)->GetRect().Contains(x, y)){
                (*it)->SetState(Control::ON_HOVER);
            }else{
                (*it)->SetState(Control::NORMAL);
            }
        }
    }
	//}

	if (event.Type == sf::Event::MouseButtonReleased)
	{
        // transormations des coords absolues en coords relatives
        int x = event.MouseButton.X - GetPosition().x;
        int y = event.MouseButton.Y - GetPosition().y;
        int r;

		if(event.MouseButton.Button == sf::Mouse::Left)
		{
			for (it=controls_.begin();it!=controls_.end();it++){
			// Pour chaque widget on verifit si une action les concerne
				if((*it)->GetRect().Contains(x, y)){
					if(r = WindowCallback((*it)->GetID(), 0, NULL))
                        return r;
				}
			}
		}
	}
	return 0;
}

void Window::Load(const std::string& xmlfile){
    int id, x=0, y=0, w, h, alpha;
    const char* p = NULL;
    std::string temp1, temp2;

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
		if (elem->QueryIntAttribute("alpha", &alpha) != TIXML_SUCCESS){
		    alpha = -1;
		}
        p = elem->Attribute("background");
        if(p != NULL){
            SetPosition(x, y);
            background_ = GET_IMG(p);
            background_.Resize(w, h);

            if(alpha != -1)
                background_.SetColor(sf::Color(255,255,255,alpha));

            rect_.Top = y;
            rect_.Bottom = h + y;
            rect_.Left = x;
            rect_.Right = h + x;
        }
    }

	node = doc.FirstChild("window")->FirstChildElement();
	controls_elem = node->ToElement();
	while (controls_elem != NULL){

        // id du widget
        if(controls_elem->QueryIntAttribute("id", &id) != TIXML_SUCCESS){
            cout << "Warning: Aucun identifiant pour ce widget" << endl;
            id = 0;
        }

	    // Position du widget (n�c�ssaire � tout les contr�les)
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

        // Taille souhait� du widget
        if (controls_elem->QueryIntAttribute("w", &w) != TIXML_SUCCESS)
			w = -1;
		if (controls_elem->QueryIntAttribute("h", &h) != TIXML_SUCCESS)
			h = -1;

        Control::ControlPos ctrl_pos(x, y);
	    Control::ControlPos ctrl_size(w, h);

	    std::string s1 = controls_elem->Value();

	    if(s1 == "button"){
            p = controls_elem->Attribute("pic");
            if(p == NULL){
                temp1 = "";
            }else{
                temp1 = p;
            }

            p = controls_elem->Attribute("OnHover");
            if(p == NULL){
                temp2 = "";
            }else{
                temp2 = p;
            }

            controls_.push_back(new Button(id, ctrl_pos, ctrl_size, temp1, temp2));
	    }else if(s1 == "label"){
	        controls_.push_back(new Label(id, ctrl_pos, controls_elem->GetText()));
	    }
	    else if (s1 == "textbox")
	    {
	        // TODO: Ajout possible d'un texte par d�faut ?
	    	controls_.push_back(new TextBox(id, ctrl_pos));
	    }

		controls_elem = controls_elem->NextSiblingElement();
	}
}

void Window::UnLoad(){
    std::vector<Control*>::iterator it;
    for(it=controls_.begin();it!=controls_.end();it++){
        delete (*it);
    }
    controls_.clear();
}

void Window::Render(sf::RenderTarget& app) const{
    std::vector<Control*>::const_iterator it;

    app.Draw(background_);
    for(it=controls_.begin();it!=controls_.end();it++){
        app.Draw(*(*it));
    }
}