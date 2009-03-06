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

class Window{
public:
    Window();
    Window(const Window& other);
    ~Window();

    /**
    * Chare une fenêtre à partir d'un fichier xml
    * param[in] xmlfile Nom du fichier XML à ouvrir
    */
    void Load(const std::string& xmlfile);

    /**
    * Affichage de la fenêtre
    */
	void Show(sf::RenderTarget& app);

	/**
	* Gestion des evenements concernant la fenêtre
	*/
	void ManageEvent(const sf::Event& event);
private:
    /**
    * Image de fond de la fenêtre
    */
    sf::Sprite background_;

    /**
    * Position de la fenêtre
    */
    sf::Vector2f position_;

    /**
    * Control composant la fenêtre
    */
    std::vector<Control*> controls_;

};

#endif
