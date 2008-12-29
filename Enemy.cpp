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
	sf::Vector2f pos = GetPosition();
	pos.x = pos.x + speed_ * frametime;
	sf::FloatRect rect;
	rect.Left = pos.x;
	rect.Bottom = pos.y;
	rect.Right = pos.x + GetFloorWidth();
	rect.Top = pos.y - GetFloorHeight();
	if (zone_->CanMove(rect))
	{
		SetX(pos.x);
	}
	else
	{
		speed_ *= -1;
	}
}

