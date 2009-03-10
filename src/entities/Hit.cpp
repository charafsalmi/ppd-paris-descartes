#include "Hit.hpp"
#include "../misc/MediaManager.hpp"
#include "../core/Zone.hpp"


Hit::Hit(const sf::Vector2f& position, int damage, Direction dir) :
	Entity(position, GET_IMG("items"))
{
	damage_ = damage;
	/*
	switch (move)
	{
		case LINEAR:
			update_callback_ = &Hit::MoveLinear;
			break;
		case CIRCULAR:
			update_callback_ = &Hit::MoveCircular;
			break;
	}*/
	switch (dir)
	{
		case DOWN:
			SetRotation(90);
			break;
		case RIGHT:
			SetRotation(180);
			break;
		case UP:
			SetRotation(270);
			break;
		default:
			break;
	}
	update_callback_ = &Hit::MoveLinear;
	direction_ = dir;
	speed_ = 250;
	SetSubRect(sf::IntRect(34, 0, 34 + 30, 0 + 10));
	SetFloor(30, 10);
	SetCenter(0, 10);
}


void Hit::Update(float frametime)
{
	(this->*update_callback_)(frametime);
	const sf::FloatRect& rect = GetRect();
	if (!zone_->CanMove(rect))
	{
		Kill();
	}
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
			//printf("Hit move linear up %f\n", frametime);
			{
			int y = GetPosition().y - speed_ * frametime;
			SetY(y);
			}
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


void Hit::MoveCircular(float frametime)
{
	// TODO
}
