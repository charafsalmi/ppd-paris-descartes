#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <string>
#include <SFML/Graphics.hpp>

namespace gui{
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

        inline sf::IntRect GetPosition(){
            return rect_;
        }
        inline ControlID GetID(){
            return id_;
        }
    protected:
        ControlID id_;
        /**
        * Position du contrôle
        */
        sf::IntRect rect_;
    };
}

#endif
