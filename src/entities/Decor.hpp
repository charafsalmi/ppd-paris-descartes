#ifndef DECOR_HPP
#define DECOR_HPP

#include "Entity.hpp"

/**
 * Élément de décoration fixe
 */
class Decor: public Entity
{
public:
	/**
	 * @param[in] floor: dimensions du rectangle de contact avec le sol
	 * NULL si le rectangle recouvre tout le sprite
	 */
	Decor(const sf::Vector2f& pos, const sf::Image& image,
		const sf::Vector2i* floor=NULL);
	
	inline void TakeDamage(int damage)
	{
		(void) damage;
	}
};

#endif /* DECOR_HPP */

