#include "Equipable.hpp"


Equipable::Equipable(const sf::Vector2f& pos):
	Item(pos, "<None>")
{
}


void Equipable::OnCollide(Player& player) { (void) player; }
