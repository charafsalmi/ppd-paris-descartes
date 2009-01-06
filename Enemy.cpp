#include "Enemy.hpp"
#include "MediaManager.hpp"
#include "Zone.hpp"

#define SPEED 80


Enemy::Enemy(const sf::Vector2f& pos) :
	Entity(pos, GET_IMG("badguy")),
	Animated(&GET_ANIM("stalfos_walk_right"), *this)
{
	SetFloor(32, 32);
	speed_ = SPEED;
	
	walk_anims_[UP]		= const_cast<Animation*>(&GET_ANIM("stalfos_walk_top"));
	walk_anims_[DOWN]	= const_cast<Animation*>(&GET_ANIM("stalfos_walk_bottom"));
	walk_anims_[LEFT]	= const_cast<Animation*>(&GET_ANIM("stalfos_walk_left"));
	walk_anims_[RIGHT]	= const_cast<Animation*>(&GET_ANIM("stalfos_walk_right"));
	
	current_dir_ = RIGHT;
	
	
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
		Animated::Update(frametime, *this);
		SetX(rect.Left);
	}
	else
	{
		speed_ *= -1;
		if (current_dir_ == LEFT)
		{
			current_dir_ = RIGHT;
		}
		else
		{
			current_dir_ = LEFT;
		}
		
		Animated::Change(walk_anims_[current_dir_], *this);
	}
}

