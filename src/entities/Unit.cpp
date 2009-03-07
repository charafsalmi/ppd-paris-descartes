#include "Unit.hpp"

#define BLEED_DELAY 0.5f


Unit::Unit(const sf::Vector2f& position, const sf::Image& image) :
	Entity(position, image)
{
	SetFloor(32, 32); // FIXME: magic

	for (int i = 0; i < COUNT_DIRECTION; ++i)
	{
		walk_anims_[i] = NULL;
	}
	bleed_timer_ = 0;
	bleeding_ = BLEED_STOP;
}


void Unit::Update(float frametime)
{
	// dégradé vers le rouge (255, 0, 0) puis retour à (255, 255, 255)
	if (bleeding_ != BLEED_STOP)
	{
		sf::Uint8 rate = (sf::Uint8) (255 * bleed_timer_ / BLEED_DELAY);
		if (bleeding_ == BLEED_IN)
		{
			if (bleed_timer_ > BLEED_DELAY)
			{
				bleeding_ = BLEED_OUT;
				bleed_timer_ = 0;
			}
			else
			{
				rate = 255 - rate;
				SetColor(sf::Color(255, rate, rate));
			}
		}
		else
		{
			SetColor(sf::Color(255, rate, rate));
			if (bleed_timer_ > BLEED_DELAY)
			{
				bleeding_ = BLEED_STOP;
				SetColor(sf::Color::White);
			}
		}
		bleed_timer_ += frametime;
	}
}


void Unit::TakeDamage(int damage)
{
	hp_ -= damage;
	if (hp_ <= 0)
	{
		Kill();
	}
	else
	{
		bleed_timer_ = 0;
		bleeding_ = BLEED_IN;
	}
}


void Unit::SetHP(int hp)
{
	hp_ = hp;
}


void Unit::SetAnimation(Direction dir, const Animation* anim)
{
	walk_anims_[dir] = anim;
}

