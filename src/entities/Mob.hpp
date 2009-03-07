#ifndef MOB_HPP
#define MOB_HPP

#include "Unit.hpp"

/**
 * Entité Autonome Potentiellement Hostile :D
 */
class Mob: public Unit
{
public:
	Mob(const sf::Vector2f& pos, const sf::Image& image, int hp, int speed);

	// inherited
	void Update(float frametime);

	// inherited
	void OnCollide(Entity& entity);

private:
	int speed_;
	Direction current_dir_;
};

#endif /* MOB_HPP */

