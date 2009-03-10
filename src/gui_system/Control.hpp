#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <string>
#include <SFML/Graphics.hpp>

namespace gui{
    class Control: public sf::Drawable{
    public:
        /**
        * Typedef
        */
        typedef unsigned int ControlID;
        typedef sf::Vector2f ControlPos;

        Control();
        Control(const ControlID id, const ControlPos& pos);
        virtual ~Control();

        /** Récupère la zone du widget */
        inline const sf::IntRect& GetRect() const {
            return rect_;
        }
        /** Récupère son identifiant */
        inline ControlID GetID() const{
            return id_;
        }
    protected:
        /** Identifiant du contrôle */
        ControlID id_;
        /**
        * Zone du contrôle
        */
        sf::IntRect rect_;
	private:
		virtual void Render(sf::RenderTarget& app) const = 0;
    };
}

#endif
