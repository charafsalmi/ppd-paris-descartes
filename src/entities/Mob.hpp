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
	void OnCollide(Entity& entity);

	void SetDirection(Direction dir);

private:
	// inherited
	void AutoUpdate(float frametime);

	// inherited
	void ThrowHit();

	float last_hit_;
	int speed_;
	Direction current_dir_;
};

#endif /* MOB_HPP */

