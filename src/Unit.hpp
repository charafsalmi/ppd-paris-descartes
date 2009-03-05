#ifndef UNIT_HPP
#define UNIT_HPP

#include "Entity.hpp"
#include "Animated.hpp"


class Unit: public Entity, public Animated
{
public:
	Unit(const sf::Vector2f& pos, const sf::Image& image);
	
	void Update(float frametime) = 0;
	
	void SetHP(int hp);
	
	void SetAnimation(Direction dir, const Animation* anim);
	
protected:
	// Animations de déplacement
	const Animation* walk_anims_[COUNT_DIRECTION];
	int hp_;
};


#endif /* UNIT_HPP */

