#ifndef DECOR_HPP
#define DECOR_HPP

#include "Entity.hpp"

/**
 * Élément de décoration fixe
 */
class Decor: public Entity
{
public:
	Decor(const sf::Vector2f& pos, const sf::Image& image);

	// inherited
	void TakeDamage(int)
	{
	}

	// inherited
	void GetCollideRect(sf::FloatRect& rect) const;
};

#endif // DECOR_HPP

