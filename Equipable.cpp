#include "Equipable.hpp"

Equipable::Equipable(ItemData& data) :
	Item(data)
{
	equipable_ = true;
}
