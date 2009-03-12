#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Control.hpp"

namespace gui{
    class Button : public Control{
    public:
        Button(const ControlID id, const ControlPos& pos, const ControlPos& size, const std::string &imagePath, const std::string &hoverPath);

        void ChangeSprite(const sf::Sprite& nimg);

    private:
		void Render(sf::RenderTarget& app) const;

        /** Medias */
        sf::Sprite img_;
        sf::Sprite img_hover_;
    };
}

#endif
