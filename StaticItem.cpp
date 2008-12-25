#include "StaticItem.hpp"
#include "Tile.hpp"


StaticItem::StaticItem(const sf::Vector2f& pos, const sf::Image& image,
	const sf::Vector2i* floor) :
	Entity(pos, image)
{
	if (floor != NULL)
	{
		SetFloor(floor->x, floor->y);
	}
}

