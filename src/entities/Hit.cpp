#include "Hit.hpp"
#include "../misc/MediaManager.hpp"


Hit::Hit(const sf::Vector2f& position, int damage) :
	Entity(position, GET_IMG("items"))
{
	damage_ = damage;
	update_callback_ = &Hit::MoveLinear;
}


void Hit::Update(float frametime)
{
	(this->*update_callback_)(frametime);
}


void Hit::OnCollide(Entity& entity)
{
	entity.TakeDamage(damage_);
	Kill();
}


void Hit::TakeDamage(int damage)
{
	(void) damage;
}


void Hit::MoveLinear(float frametime)
{
	switch (direction_)
	{
		case UP:
			Move(0, -speed_ * frametime);
			break;
		case DOWN:
			Move(0, speed_ * frametime);
			break;
		case LEFT:
			Move(-speed_ * frametime, 0);
			break;
		case RIGHT:
			Move(speed_ * frametime, 0);
			break;
		default:
			break;
	}
}
