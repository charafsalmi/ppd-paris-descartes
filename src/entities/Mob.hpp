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

	~Mob();

	// inherited
	void Kill();

	// inherited
	void TakeDamage(int damage);

	/**
	 * Choisir une direction au hasard
	 */
	void ChooseDirection();

	// inherited
	void SetEquipment(Equipment* equipment);

private:
	// inherited
	void AutoUpdate(float frametime);

	/**
	 * Supprimer l'item courant
	 */
	void RemoveEquipment();

	Equipment* equipment_;

	float walk_duration_;
	float started_at_;
};

#endif // MOB_HPP

