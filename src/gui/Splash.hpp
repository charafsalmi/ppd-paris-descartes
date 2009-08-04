#ifndef SPLASH_HPP
#define SPLASH_HPP

#include <SFML/Graphics.hpp>

/**
 * Écran d'accueil avec effet de transition
 */
class Splash
{
public:
	Splash(sf::RenderWindow& app);

	/**
	 * @return false si demande de fermeture de l'application
	 */
	bool Run();

private:
	bool Update(float frametime);

	void Draw();

	sf::Sprite sprite_;
	sf::PostFX fx_;
	sf::RenderWindow& app_;
};

#endif // SPLASH_HPP
