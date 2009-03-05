#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <string>
#include <SFML/Graphics.hpp>

class Control{
public:
    /**
    * Typedef
    */
    typedef unsigned int ControlID;
    typedef sf::Vector2f ControlPos;

    Control();
    Control(ControlID id, ControlPos pos);
    virtual ~Control();

    virtual void Show(sf::RenderTarget& app)=0;

protected:
    ControlID id_;
private:
    /**
    * Position du contrôle
    */
    sf::Vector2f position_;
};

#endif
