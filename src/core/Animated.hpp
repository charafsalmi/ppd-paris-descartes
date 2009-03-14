#ifndef ANIMATED_HPP
#define ANIMATED_HPP

#include <SFML/Graphics.hpp>

#include "Animation.hpp"

/**
 * Interface de sprite animé
 */
class Animated
{
public:
	Animated();

	Animated(const Animation* animation, sf::Sprite& sprite);

	/**
	 * Mettre à jour le subrect d'un sprite animé
	 * @param[in] frametime: temps de la frame courante
	 * @param[out] sprite: sprite à mettre à jour
	 */
	void Update(float frametime, sf::Sprite& sprite);

protected:
	/**
	 * Modifier la séquence de l'animation
	 * @param[in] animation: nouvelle animation
	 * @param[in] sprite: entité animée
	 */
	void Change(const Animation* animation, sf::Sprite& sprite);

private:
	const Animation* animation_;
	int frame_;
	float timer_;
};

#endif /* ANIMATED_HPP */

