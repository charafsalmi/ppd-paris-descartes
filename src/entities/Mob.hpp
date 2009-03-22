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

	/// inherited
	void Kill();

	/// inherited
	void TakeDamage(int damage);

	/**
	 * Choisir une direction au hasard
	 */
	void ChooseDirection();

private:
	/// inherited
	void AutoUpdate(float frametime);

	/// inherited
	void ThrowHit();

	float last_hit_;
	float walk_duration_;
	float started_at_;
};

#endif /* MOB_HPP */

