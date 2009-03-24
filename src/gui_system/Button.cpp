#include "Button.hpp"
#include "../misc/MediaManager.hpp"

using namespace gui;
using namespace std;

Button::Button(ControlID id, const ControlPos& pos, const ControlPos& size, const std::string &imagePath, const std::string &hoverPath) : Control(id, pos)
{
    ControlPos s = size;

    // Mode avec image
    if(!imagePath.empty()){
        img_ = GET_IMG(imagePath.c_str());

        // Si aucune largeur ou hauteur n'est définit,
        // on récupère celles de l'image
        if(s.x == UNDEFINED || s.y == UNDEFINED){
            s.x = img_.GetSize().x;
            s.y = img_.GetSize().y;
        }
        hidden_ = false;
    }else{
        hidden_ = true;
    }

    // Système OnHover
    if(!hoverPath.empty()){
        img_hover_ = GET_IMG(hoverPath.c_str());

        accepted_states_ |= ON_HOVER;
        // On applique la même position que l'image de base
        //img_hover_.SetPosition(pos.x, pos.y);
    }

    // Positionnement du contrôle
    // Ce rectangle sert à calculer les points d'intercept.
    rect_.Top = pos.y;
    rect_.Left = pos.x;
    rect_.Bottom = pos.y + s.y;
    rect_.Right = pos.x + s.x;
}

void Button::Render(sf::RenderTarget& app) const
{
    if((accepted_states_ & ON_HOVER)&&(curr_state_ == ON_HOVER)){
        app.Draw(img_hover_);
    }else{
        if(!hidden_)
            app.Draw(img_);
    }
}

void Button::ChangeSprite(const sf::Sprite& nimg)
{
    ControlPos p = img_.GetPosition();

    img_ = nimg;
    img_.Resize(rect_.Right-rect_.Left, rect_.Bottom-rect_.Top);
    img_.SetPosition(p.x, p.y+img_.GetSize().y);

    hidden_ = false;
}
