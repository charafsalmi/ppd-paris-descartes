#ifndef UNIT_HPP
#define UNIT_HPP

#include "Entity.hpp"
#include "../core/Animated.hpp"


class Equipment;

class Unit: public Entity, public Animated
{
public:
	/**
	 * @param[in] pos: position en pixels
	 * @param[in] image: image du charset
	 * @param[in] hp: points de vie
	 * @param[in] speed: vitesse en pixels/seconde
	 */
	Unit(const sf::Vector2f& pos, const sf::Image& image, int hp, float speed);

	// inherited
	void Update(float frametime) ;

	// inherited
	void TakeDamage(int damage);

	// inherited
	virtual void OnCollide(Entity& entity, const sf::FloatRect& overlap);

	// inherited
	CollideEffect GetCollideEffect() const;

	// inherited
	bool IsDying() const;

	virtual void SetEquipment(Equipment* equipment) = 0;

	void SetAnimation(Direction dir, const Animation* anim);

	/**
	 * Orientation courante de l'entité
	 */
	Direction GetDirection() const;

	int GetHP() const;
	virtual void SetHP(int hp);

protected:
	virtual void AutoUpdate(float frametime) = 0;

	void SetDirection(Direction dir);

	inline float GetSpeed() const
	{
		return speed_;
	}

	inline void SetSpeed(float speed)
	{
		speed_ = speed;
	}

	// Animations de déplacement
	// TODO: private
	const Animation* walk_anims_[COUNT_DIRECTION];
private:
	void DyingUpdate(float frametime);

	enum Bleeding
	{
		BLEED_IN, BLEED_OUT, BLEED_STOP
	};
	Bleeding bleeding_;
	float timer_;
	int hp_;
	float speed_;
	Direction current_dir_;

	bool is_knocked_;
	Direction knocked_dir_;
	float knocked_speed_;
	float knocked_start_;

	void (Unit::*update_callback_)(float frametime);


};

#endif // UNIT_HPP

