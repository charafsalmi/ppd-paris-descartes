#ifndef HIT_HPP
#define HIT_HPP

#include "Entity.hpp"


class Hit: public Entity
{
public:
	enum Movement // ?
	{
		LINEAR, CIRCULAR
	};


	Hit(const sf::Vector2f& position, int damage, Direction dir, int emitter_id_);

	// inherited
	void Update(float frametime);

	// inherited
	virtual void OnCollide(Entity& entity);

	// inherited
	void TakeDamage(int damage);

	/**
	 * Obtenir l'id de l'entité qui a émis le hit
	 */
	int GetEmitterID() const;

private:
	/**
	 * Déplacement linéaire
	 */
	void MoveLinear(float frametime);

	void MoveCircular(float frametime);

	void (Hit::*update_callback_)(float frametime);

	Direction direction_;
	int damage_;
	float speed_;
	int emitter_id_;
};


#endif /* HIT_HPP */

