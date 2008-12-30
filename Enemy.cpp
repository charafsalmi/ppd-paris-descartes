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
	if(zone_->CanMove(rect))
	{
		SetX(pos.x);
	}
	else
	{
		speed_ *= -1;
	}
}

bool Enemy::Move(CUSINT dir, float frametime)
{
	//@TODO : à compléter
	return false;
}

bool Enemy::MoveUp(float frametime)
{
	return Move(UP, frametime);
}

bool Enemy::MoveRight(float frametime)
{
	return Move(RIGHT, frametime);
}

bool Enemy::MoveDown(float frametime)
{
	return Move(DOWN, frametime);
}

bool Enemy::MoveLeft(float frametime)
{
	return Move(LEFT, frametime);
}

bool Enemy::MoveRandomly(float frametime)
{
	srand(time(NULL));
	return Move((rand() % COUNT_DIRECTION), frametime);
}
