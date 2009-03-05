#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include "Control.hpp"

class Window{
public:
    Window();
    Window(const Window& other);
    ~Window();

	void Show(sf::RenderTarget& app);
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
