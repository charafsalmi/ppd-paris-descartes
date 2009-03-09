#ifndef HIT_HPP
#define HIT_HPP

#include "Entity.hpp"

class Hit: public Entity
{
public:
	Hit(const sf::Vector2f& position, int damage);

	// inherited
	void Update(float frametime);

	// inherited
	void OnCollide(Entity& entity);

	// inherited
	void TakeDamage(int damage);
private:

	/**
	 * Déplacement linéaire
	 */
	void MoveLinear(float frametime);

	void (Hit::*update_callback_)(float frametime);

	Direction direction_;
	int damage_;
	float speed_;
};


#endif /* HIT_HPP */

