#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Control.hpp"

class Button : public Control{
public:
    Button();
    Button(ControlID id, ControlPos pos, const std::string& str);

    ~Button();

    void Show(sf::RenderTarget& app);

private:
    sf::Sprite img_;
};

#endif
