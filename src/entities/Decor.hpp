#ifndef DECOR_HPP
#define DECOR_HPP

#include "Entity.hpp"


/**
 * Élément de décoration
 */
class Decor: public Entity
{
public:
	Decor(const sf::Vector2f& pos, const sf::Image& image);

	void SetMovable(bool movable);

	// inherited
	Entity* Clone() const;

	// inherited
	void Update(float frametime);

	// inherited
	void TakeDamage(int)
	{
	}

	// inherited
	CollideEffect GetCollideEffect() const;

	// inherited
	void OnCollide(Entity& entity, const sf::FloatRect& overlap);

private:
	enum Status
	{
		NOT_MOVABLE,
		MOVABLE,
		PUSHED,
		MOVING
	};
	Status status_;
	const Entity* pusher_;
	Direction push_direction_;
	float timer_;
};

#endif // DECOR_HPP

