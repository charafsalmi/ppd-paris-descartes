#include "Enemy.hpp"
#include "MediaManager.hpp"
#include "Zone.hpp"

#define SPEED 80


Enemy::Enemy(const sf::Vector2f& pos) :
	Entity(pos, GET_IMG("badguy"))
{
	speed_ = SPEED;
}


void Enemy::Move(float frametime)
{
	const sf::Vector2f& pos = GetPosition();
	sf::FloatRect rect;
	rect.Left = pos.x + speed_ * frametime;
	rect.Bottom = pos.y;
	rect.Right = rect.Left + GetFloorWidth();
	rect.Top = rect.Bottom - GetFloorHeight();
	if(zone_->CanMove(this, rect))
	{
		SetX(rect.Left);
	}
	else
	{
		speed_ *= -1;
	}
}

