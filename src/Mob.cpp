#include <cstring>

#include "Mob.hpp"
#include "Zone.hpp"

#define SPEED 80


Mob::Mob(const sf::Vector2f& pos, const sf::Image& image) :
	Unit(pos, image)
{
	speed_ = SPEED;
	current_dir_ = RIGHT;
}


void Mob::Update(float frametime)
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

