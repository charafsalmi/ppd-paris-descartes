#ifndef STATICITEM_HPP
#define STATICITEM_HPP

#include "Entity.hpp"

/**
 * Élément immobile
 */
class StaticItem: public Entity
{
public:
	/**
	 * @param[in] floor: dimensions du rectangle de contact avec le sol
	 * NULL si le rectangle recouvre tout le sprite
	 */
	StaticItem(const sf::Vector2f& pos, const sf::Image& image,
		const sf::Vector2i* floor=NULL);
};

#endif /* STATICITEM_HPP */

