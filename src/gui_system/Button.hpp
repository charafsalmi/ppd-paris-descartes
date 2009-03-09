#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Control.hpp"

namespace gui{
    class Button : public Control{
    public:
        Button();
        Button(const ControlID id, const ControlPos& pos, const std::string& str);

        void Show(sf::RenderTarget& app);

    private:
        sf::Sprite img_;
    };
}

#endif
