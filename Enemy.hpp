#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"

class Enemy: public Entity
{
public:
	Enemy(const sf::Vector2f& pos);
	
	void Move(float frametime);
	
private:
	int speed_;
};

#endif /* ENEMY_HPP */

