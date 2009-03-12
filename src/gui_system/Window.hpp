#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>

#include "Control.hpp"

/*
    Il existe deux types de fenêtres. Les fenêtres "bloquantes" qui contienne
dans leurs code une méthode pour récupéré les events. Et une fenêtre non-bloquante
qui s'intercale dans le système d'event du jeu. Grace à cette procédure, le jeu continue
alors que la fenêtre s'affiche.
*/

/* TODO: Passage en forme canonique */
namespace gui{
    enum ID
    {
        IDEXIT = 7771, IDCONFIRM
    };

    class Window: public sf::Drawable
    {
    public:
        Window();
        Window(const Window& other);
        virtual ~Window();

        /**
        * Callback du gestionnaire de fenêtre
        */
        virtual int WindowCallback(const Control::ControlID id, const int p1, void *p2){return 0;};

        /**
        * Gestion des evenements concernant la fenêtre
        */
        int ManageEvent(const sf::Event& event);

    protected:
        /**
        * Récupère un contrôle à partir de son id
        */
        Control *GetFromID(Control::ControlID id);

        /**
        * Charge une fenêtre à partir d'un fichier xml
        * param[in] xmlfile Nom du fichier XML à ouvrir
        */
        void Load(const std::string& xmlfile);

        /**
        * Déchargement de tout les composants
        */
        void UnLoad();
    private:
		/**
         * Affichage de la fenêtre
         */
		virtual void Render(sf::RenderTarget& app) const;

		/**
		 * Récupérer le contrôle sous la souris
		 * @param[in] x: position absolue x
		 * @param[in] y: position absolue y
		 * @return le contrôle sous la souris, NULL si aucun
		 */
		Control* GetUnderMouse(int x, int y);

        /**
        * Image de fond de la fenêtre
        */
        sf::Sprite background_;

        /**
        * Position de la fenêtre
        */
        sf::IntRect rect_;
        /**
        * Composants de la fenêtre
        */
        std::vector<Control*> controls_;
        // contrôle qui a le focus
        Control* active_;
        // contrôle sous la souris
        Control* hover_;
    };
}

#endif
