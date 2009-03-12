#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <string>
#include <SFML/Graphics.hpp>

namespace gui{
    class Control: public sf::Drawable{
    public:
		enum { UNDEFINED = -1 };

        /**
        * Typedef
        */
        typedef unsigned int ControlID;
        typedef sf::Vector2f ControlPos;

        /**
        * Etat du control
        */
        enum State{
            NORMAL=0x100,
            ON_FOCUS=0x010,
            ON_HOVER=0x001
        };

        Control(const ControlID id, const ControlPos& pos);
        virtual ~Control();

        /** Changement d'état du contrôle */
        inline void SetState(State s){
            curr_state_ = s;
        }

        /** Récupère la zone du widget */
        inline const sf::IntRect& GetRect() const {
            return rect_;
        }
        /** Récupère son identifiant */
        inline ControlID GetID() const{
            return id_;
        }

        /** Change le sprite du contrôle qui le peu */
        virtual void ChangeSprite(const sf::Sprite& nimg);

		/**
		 * Callback évènement texte saisi
		 * @param[in] unicode: caractère reçu
		 */
        virtual void OnTextEntered(sf::Uint32 unicode);

		/**
		 * Callback évènement touche pressée
		 * @param[in] key: touche appuyée
		 */
		virtual void OnKeyPressed(sf::Key::Code key);

    protected:
        /** Identifiant du contrôle */
        ControlID id_;
        /**
        * Zone du contrôle
        */
        sf::IntRect rect_;

        /**
        * Etat courant du contrôle
        */
        State curr_state_;

        /**
        * Etats réalisable par le contrôle
        */
        int accepted_states_;
	private:
		virtual void Render(sf::RenderTarget& app) const = 0;
    };
}

#endif
