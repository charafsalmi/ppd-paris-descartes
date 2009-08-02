#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <string>
#include <SFML/Graphics.hpp>

namespace gui{
    class Control: public sf::Drawable{
    public:
		enum { UNDEFINED = -1 };

        typedef unsigned int ControlID;
        typedef sf::Vector2f ControlPos;

        /// Etat du control
        enum State{
            NORMAL=0x100,
            ON_FOCUS=0x010,
            ON_HOVER=0x001
        };

        Control(const ControlID id, const ControlPos& pos, const std::string& s="");
        virtual ~Control();

        /// RÈcupËre le texte du contrÙle
        /// @return Texte du control
        virtual std::string GetText() const{
            return text_;
        }

        /// Place un texte
        virtual void SetText(const std::string s){
            text_ = s;
        }

        /// Permet d'associÈ un int au control
        void LinkInt(int *);

        /// Permet d'associÈ un char au control
        void LinkChar(char *);
		
		/// Permet d'associer un float au control
		void LinkFloat(float*);

        /// Changement d'Ètat du contrÙle
        /// @param[in] s Etat du contrÙle
        inline void SetState(State s){
            curr_state_ = s;
        }

        /// RÈcupËre la zone du widget
        inline const sf::IntRect& GetRect() const {
            return rect_;
        }
        /// RÈcupËre son identifiant
        inline ControlID GetID() const{
            return id_;
        }

        /// Change le sprite du contrÙle qui le peu
        virtual void ChangeSprite(const sf::Sprite& nimg);

        /// Callback ÈvËnement texte saisi
        /// @param[in] unicode: caractËre reÁu
        virtual void OnTextEntered(sf::Uint32 unicode);

        /// Callback ÈvËnement touche pressÈe
        /// @param[in] key: touche appuyÈe
		virtual void OnKeyPressed(sf::Key::Code key);

		/// Mise ‡ jour du control
        virtual void Update(){}
    protected:
        /// Identifiant du contrÙle
        ControlID id_;

        /// Zone du contrÙle
        sf::IntRect rect_;

        /// Etat courant du contrÙle
        State curr_state_;

        /// Etats rÈalisable par le contrÙle
        int accepted_states_;

        /// Mode du control
        enum Mode{
            PLAIN, LINKED_INT, LINKED_CHAR, LINKED_FLOAT
        };

        /// Mode du label
        Mode mode_;

        /// Pointeur du mode linked
        void *ptr_;

        /// Texture du contrÙle
        mutable std::string text_;
	private:
		virtual void Render(sf::RenderTarget& app) const = 0;
    };
}

#endif
