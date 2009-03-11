#include "Decor.hpp"


Decor::Decor(const sf::Vector2f& pos, const sf::Image& image) :
	Entity(pos, image)
{
		printf("decor en %d, %d\n", (int)pos.x, (int)pos.y);
}

