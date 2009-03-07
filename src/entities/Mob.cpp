#include "Mob.hpp"
#include "../core/Zone.hpp"


Mob::Mob(const sf::Vector2f& pos, const sf::Image& image, int hp, int speed) :
	Unit(pos, image)
{
	hp_ = hp;
	speed_ = speed;
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
	if (zone_->CanMove(rect))
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


void Mob::OnCollide(Entity& entity)
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
