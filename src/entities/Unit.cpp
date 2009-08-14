#include "Unit.hpp"
#include "Hit.hpp"
#include "../core/Game.hpp"
#include "../core/Zone.hpp"

#define BLEED_DELAY         0.5f
#define DYING_DELAY         3.0f
#define KNOCK_DELAY         0.8f
#define KNOCK_INITIAL_SPEED 400
#define KNOCK_SLOW_FACTOR   0.9


Unit::Unit(const sf::Vector2f& position, const sf::Image& image, int hp, float speed) :
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
	is_knocked_ = false;
	speed_ = speed;
	hp_ = hp;
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
	if (is_knocked_)
	{
		if ((Game::GetInstance().GetElapsedTime() - knocked_start_) > KNOCK_DELAY)
		{
			is_knocked_ = false;
		}
		Move(knocked_dir_, knocked_speed_ * frametime, Tile::DEFAULT);
		knocked_speed_ *= KNOCK_SLOW_FACTOR;
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


void Unit::OnCollide(Entity& entity, const sf::FloatRect& overlap)
{
	if (IsDying() || entity.IsDying())
	{
		return;
	}
	switch (entity.GetCollideEffect())
	{
		case FX_REJECTION:
			// repoussement horizontal ?
			if (overlap.GetHeight() < overlap.GetWidth())
			{
				// vers le haut ou le bas
				knocked_dir_ = entity.GetPosition().y > GetPosition().y ? UP : DOWN;
			}
			else // vertical
			{
				// vers la gauche ou la droite
				knocked_dir_ = entity.GetPosition().x > GetPosition().x ? LEFT : RIGHT;
			}
			knocked_start_ = Game::GetInstance().GetElapsedTime();
			knocked_speed_ = KNOCK_INITIAL_SPEED;
			is_knocked_ = true;
			break;

		case FX_STOP:
			{
				Direction dir;
				float dist;
				if (overlap.GetHeight() < overlap.GetWidth())
				{
					dir = entity.GetPosition().y > GetPosition().y ? UP : DOWN;
					dist = overlap.GetHeight();
				}
				else
				{
					dir = entity.GetPosition().x > GetPosition().x ? LEFT : RIGHT;
					dist = overlap.GetWidth();
				}
				Move(dir, dist);
			}
			break;
		case FX_NOTING:
			break;
	}
}


Entity::CollideEffect Unit::GetCollideEffect() const
{
	return FX_REJECTION;
}


bool Unit::IsDying() const
{
	return update_callback_ == &Unit::DyingUpdate;
}


void Unit::SetAnimation(Direction dir, const Animation* anim)
{
	walk_anims_[dir] = anim;
}


void Unit::SetHP(int hp)
{
	hp_ = hp;
}


int Unit::GetHP() const
{
	return hp_;
}


Entity::Direction Unit::GetDirection() const
{
	return current_dir_;
}


void Unit::SetDirection(Direction dir)
{
	current_dir_ = dir;
	Animated::Change(walk_anims_[dir], *this);
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

