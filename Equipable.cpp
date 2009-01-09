#include "Equipable.hpp"

Equipable::Equipable(sf::Vector2f& offset, ItemData& data) :
	Item(offset, data)
{
	Item::equipable_ = true;
}
