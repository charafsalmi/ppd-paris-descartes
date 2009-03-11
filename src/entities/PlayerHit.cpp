#include <typeinfo>

#include "PlayerHit.hpp"
#include "Player.hpp"


PlayerHit::PlayerHit(const sf::Vector2f position, int damage, Direction dir) :
	Hit(position, damage, dir)
{
	printf("le hero se prend des dommages");
}


void PlayerHit::OnCollide(Entity& entity)
{
	if (typeid (entity) != typeid (Player))
	{
		Hit::OnCollide(entity);
	}
}
