#include <typeinfo>

#include "PlayerHit.hpp"
#include "Player.hpp"


PlayerHit::PlayerHit(const sf::Vector2f position, int damage, Direction dir, int emitter_id_) :
	Hit(position, damage, dir, emitter_id_)
{
}


void PlayerHit::OnCollide(Entity& entity)
{
	Hit::OnCollide(entity);
	// TODO: notifier le résultat à player ? (compteur de frags, etc.)
}
