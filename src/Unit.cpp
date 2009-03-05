#include "Unit.hpp"


Unit::Unit(const sf::Vector2f& position, const sf::Image& image) :
	Entity(position, image)
{
	SetFloor(32, 32); // magic
	
	for (int i = 0; i < COUNT_DIRECTION; ++i)
	{
		walk_anims_[i] = NULL;
	}
}


void Unit::SetHP(int hp)
{
	hp_ = hp;
}


void Unit::SetAnimation(Direction dir, const Animation* anim)
{
	walk_anims_[dir] = anim;
}

