#include <iostream>

#include "Button.hpp"
#include "../misc/MediaManager.hpp"

using namespace gui;
using namespace std;

Button::Button(ControlID id, const ControlPos& pos, const ControlPos& size, const std::string& str) : Control(id, pos)
{
    ControlPos s = size;

    // Mode avec image
    if(!str.empty()){
        img_ = GET_IMG(str.c_str());

        if(s.x == -1 || s.y == -1){
            s.x = img_.GetSize().x;
            s.y = img_.GetSize().y;
        }
    }

    // Positionnement du contrôle
    rect_.Top = pos.y;
    rect_.Left = pos.x;
    rect_.Bottom = pos.y + s.y;
    rect_.Right = pos.x + s.x;
}

void Button::Render(sf::RenderTarget& app) const
{
    app.Draw(img_);
}

void Button::ChangeSprite(sf::Sprite nimg)
{
    ControlPos p = img_.GetPosition();

    img_ = nimg;
    img_.Resize(rect_.Right-rect_.Left, rect_.Bottom-rect_.Top);
    img_.SetPosition(p.x, p.y+img_.GetSize().y);
}
