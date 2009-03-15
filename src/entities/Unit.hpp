#ifndef UNIT_HPP
#define UNIT_HPP

#include "Entity.hpp"
#include "../core/Animated.hpp"


class Unit: public Entity, public Animated
{
public:
	Unit(const sf::Vector2f& pos, const sf::Image& image);

	// inherited
	void Update(float frametime) ;

	// inherited
	void TakeDamage(int damage);

	// inherited
	bool IsDying() const;

	void SetHP(int hp);

	void SetAnimation(Direction dir, const Animation* anim);

protected:
	virtual void AutoUpdate(float frametime) = 0;

	// Animations de déplacement
	const Animation* walk_anims_[COUNT_DIRECTION];
	int hp_;

private:
	void DyingUpdate(float frametime);

	enum Bleeding
	{
		BLEED_IN, BLEED_OUT, BLEED_STOP
	};
	Bleeding bleeding_;
	float timer_;

	void (Unit::*update_callback_)(float frametime);
};


#endif /* UNIT_HPP */

