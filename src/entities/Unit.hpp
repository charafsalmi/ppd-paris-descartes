#ifndef UNIT_HPP
#define UNIT_HPP

#include "Entity.hpp"
#include "../core/Animated.hpp"


class Unit: public Entity, public Animated
{
public:
	Unit(const sf::Vector2f& pos, const sf::Image& image);
	
	// cf. Entity
	void Update(float frametime) = 0;
	
	// cf. Entity
	void TakeDamage(int damage);
	
	void SetHP(int hp);

	void SetAnimation(Direction dir, const Animation* anim);

protected:
	// Animations de déplacement
	const Animation* walk_anims_[COUNT_DIRECTION];
	int hp_;

private:
	enum Bleeding
	{
		BLEED_IN, BLEED_OUT, BLEED_STOP
	};
	Bleeding bleeding_;
	float bleed_timer_;
};


#endif /* UNIT_HPP */

