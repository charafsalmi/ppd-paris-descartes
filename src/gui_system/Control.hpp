#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <SFML/Graphics.hpp>

class Control{
public:
    Control();
    virtual ~Control();

    virtual void Show(sf::RenderTarget& app)=0;
private:
    /**
    * Position du control
    */
    sf::Vector2f position_;
};

#endif
