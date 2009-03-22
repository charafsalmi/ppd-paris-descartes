#ifndef UNIT_HPP
#define UNIT_HPP

#include "Entity.hpp"
#include "../core/Animated.hpp"


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

	/// inherited
	void Update(float frametime) ;

	/// inherited
	void TakeDamage(int damage);

	/// inherited
	virtual void OnCollide(Entity& entity, const sf::FloatRect& overlap);

	/// inherited
	bool IsDying() const;

	void SetAnimation(Direction dir, const Animation* anim);

	/**
	 * Déplacer l'unité dans une direction (si possible)
	 * @param[in] dir: direction souhaitée
	 * @param[in] distance: distance en pixels
	 * @param[in] tiles: types de tiles que l'on peut franchir
	 */
	void Move(Direction dir, int distance, int tiles);

protected:
	virtual void AutoUpdate(float frametime) = 0;

	int GetHP() const;

	void SetHP(int hp);

	Direction GetDirection() const;

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


#endif /* UNIT_HPP */

