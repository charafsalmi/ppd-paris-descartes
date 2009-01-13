#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"
#include "Animated.hpp"

class Enemy: public Entity, public Animated
{
public:
	Enemy(const sf::Vector2f& pos);

	void Update(float frametime);

private:
	int speed_;
	Direction current_dir_;
	// Animations de déplacement
	const Animation* walk_anims_[COUNT_DIRECTION];
};

#endif /* guard ENEMY_HPP */

