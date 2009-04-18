#include "PlayerHit.hpp"
#include "Player.hpp"
#include "Mob.hpp"
#include "../core/Game.hpp"


PlayerHit::PlayerHit(const sf::Vector2f position, int damage, Direction dir, int emitter_id_, Hit::Type type) :
	Hit(position, damage, dir, emitter_id_, type)
{
}


void PlayerHit::OnCollide(Entity& entity, const sf::FloatRect& overlap)
{
	if (IsDying() || entity.IsDying())
	{
		return;
	}
	Hit::OnCollide(entity, overlap);
	if (dynamic_cast<Mob*>(&entity) != NULL && entity.IsDying())
	{
		Game::GetInstance().GetPlayer()->AddFrag();
	}
}
