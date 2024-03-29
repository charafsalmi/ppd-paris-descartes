#ifndef HIT_HPP
#define HIT_HPP

#include "Entity.hpp"


class Hit: public Entity
{
public:
	enum Type
	{
		SWORD, ARROW
	};

	Hit(const sf::Vector2f& position, int damage, Direction dir, int emitter_id_, Type type);

	// inherited
	void Update(float frametime);

	// inherited
	virtual void OnCollide(Entity& entity, const sf::FloatRect& overlap);

	// inherited
	bool CanFloorCollide() const;

	// inherited
	void TakeDamage(int damage);

	// inherited
	bool IsDying() const;

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

	void DyingUpdate(float update);

	void (Hit::*update_callback_)(float frametime);

	float timer_;
	bool rotate_when_dying_;
	Direction direction_;
	int damage_;
	float speed_;
	int emitter_id_;
	bool timed_;
	float time_to_live_;
};


#endif // HIT_HPP

