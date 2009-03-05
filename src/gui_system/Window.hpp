#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include "../misc/MediaManager.hpp"
#include "Control.hpp"

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
	void ManageEvent(sf::Event event);
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

	#ifdef _DEBUG
	bool _dbg;
	#endif
};

#endif
