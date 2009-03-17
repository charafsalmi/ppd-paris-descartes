#include "Unit.hpp"

#define BLEED_DELAY 0.5f
#define DYING_DELAY 3.0f


Unit::Unit(const sf::Vector2f& position, const sf::Image& image) :
	Entity(position, image)
{
	SetFloor(32, 32); // FIXME: magic

	for (int i = 0; i < COUNT_DIRECTION; ++i)
	{
		walk_anims_[i] = NULL;
	}
	timer_ = 0;
	bleeding_ = BLEED_STOP;
	update_callback_ = &Unit::AutoUpdate;
}


void Unit::Update(float frametime)
{
	// dégradé vers le rouge (255, 0, 0) puis retour à (255, 255, 255)
	if (bleeding_ != BLEED_STOP)
	{
		sf::Uint8 rate = (sf::Uint8) (255 * timer_ / BLEED_DELAY);
		if (bleeding_ == BLEED_IN)
		{
			if (timer_ > BLEED_DELAY)
			{
				bleeding_ = BLEED_OUT;
				timer_ = 0;
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
			if (timer_ > BLEED_DELAY)
			{
				bleeding_ = BLEED_STOP;
				SetColor(sf::Color::White);
			}
		}
		timer_ += frametime;
	}

	// comportement spécifique à l'unité
	(this->*update_callback_)(frametime);
}


void Unit::TakeDamage(int damage)
{
	if (hp_ > 0)
	{
		hp_ -= damage;
		timer_ = 0;
		bleeding_ = BLEED_IN;
		if (hp_ <= 0)
		{
			update_callback_ = &Unit::DyingUpdate;
		}
	}
}


bool Unit::IsDying() const
{
	return update_callback_ == &Unit::DyingUpdate;
}


void Unit::SetHP(int hp)
{
	hp_ = hp;
}


int Unit::GetHP() const
{
	return hp_;
}


void Unit::SetAnimation(Direction dir, const Animation* anim)
{
	walk_anims_[dir] = anim;
}


void Unit::DyingUpdate(float frametime)
{
	if (timer_ < DYING_DELAY)
	{
		sf::Uint8 rate = (sf::Uint8) (255 * timer_ / DYING_DELAY);
		SetColor(sf::Color(255, 255, 255, 255 - rate));
		timer_ += frametime;
	}
	else
	{
		Kill();
	}
}

