#include "Hit.hpp"
#include "Unit.hpp"
#include "../misc/MediaManager.hpp"
#include "../core/SoundSystem.hpp"
#include "../core/Zone.hpp"
#include "../core/Tileset.hpp"
#include "../misc/Log.hpp"

#define SPEED             320
#define DYING_DELAY       1.0f
#define ANGLE_PER_SECOND  720
#define SLOW_FACTOR       0.9

#define ACCEPTED_TILES (Tile::DEFAULT | Tile::WATER | Tile::TELEPORT | Tile::HOLE)


Hit::Hit(const sf::Vector2f& position, int damage, Direction dir, int emitter_id, HitType type) :
	Entity(position, GET_IMG("hits"))
{
	damage_ = damage;
	sf::IntRect subrect;
	switch (type)
	{
		case LINEAR:
            OutputD << "fleche" << lEnd;
			update_callback_ = &Hit::MoveLinear;
			switch (dir)
			{
			case UP:
				// left / top / right /bottom
				subrect = sf::IntRect(30, 0, 40, 30);
				break;
			case DOWN:
				subrect = sf::IntRect(40, 0, 50, 30);
				break;
			case RIGHT:
				subrect = sf::IntRect(0, 10, 30, 20);
				break;
			case LEFT:
				subrect = sf::IntRect(0, 0, 30, 10);
				break;
			default:
				break;
			}
			SoundSystem::GetInstance().PlaySound("arrow-shot");
			timed_ = false;
			SetCenter(0, subrect.GetHeight());
			break;
		case CIRCULAR:
			// hack temporaire, en attendant d'avoir un vrai sprite de coup d'épée
			SetImage(GET_IMG("items"));
            OutputD << "epee" << lEnd;
			update_callback_ = &Hit::MoveCircular;
			subrect = sf::IntRect(16, 0, 16 + 18, 0 + 32);
			timed_ = true;
			time_to_live_ = 1.0f;
			SetCenter(0, 0);
			break;
	}
	direction_ = dir;
	speed_ = SPEED;
	SetSubRect(subrect);
	SetFloor(subrect.GetWidth(), subrect.GetHeight());

	timer_ = 0;
	rotate_when_dying_ = false;
	emitter_id_ = emitter_id;
}


void Hit::Update(float frametime)
{
	sf::FloatRect rect;
	GetCollideRect(rect);
	if (!zone_->CanMove(rect, ACCEPTED_TILES))
	{
		update_callback_ = &Hit::DyingUpdate;
	}
	else if (timed_)
	{
		time_to_live_ -= frametime;
		if (time_to_live_ <= 0)
		{
			Kill();
		}
	}
	(this->*update_callback_)(frametime);
}


void Hit::OnCollide(Entity& entity, const sf::FloatRect&)
{
	if (IsDying() || entity.IsDying())
	{
		return;
	}
	// don't kill the owner !
	if (entity.GetID() != emitter_id_)
	{
		entity.TakeDamage(damage_);
		// effet kiss-cool en cas de collision avec un autre hit
		if (dynamic_cast<Hit*>(&entity) != NULL)
		{
			SetCenter(GetSize().x / 2, GetSize().y / 2);
			rotate_when_dying_ = true;
		}
		update_callback_ = &Hit::DyingUpdate;
	}
}


bool Hit::IsDying() const
{
	return update_callback_ == &Hit::DyingUpdate;
}


void Hit::TakeDamage(int damage)
{
	(void) damage;
}


int Hit::GetEmitterID() const
{
	return emitter_id_;
}


void Hit::MoveLinear(float frametime)
{
	switch (direction_)
	{
	case UP:
		Move(0, -speed_ * frametime);
		break;
	case DOWN:
		Move(0, speed_ * frametime);
		break;
	case LEFT:
		Move(-speed_ * frametime, 0);
		break;
	case RIGHT:
		Move(speed_ * frametime, 0);
		break;
	default:
		break;
	}
}


void Hit::MoveCircular(float frametime)
{
    switch (direction_)
	{
	case UP:
		Rotate(ANGLE_PER_SECOND/2 * frametime);
		break;
	case DOWN:
		Rotate(ANGLE_PER_SECOND/2 * frametime);
		break;
	case LEFT:
		Rotate(ANGLE_PER_SECOND/2 * frametime);
		break;
	case RIGHT:
		Rotate(ANGLE_PER_SECOND/2 * frametime);
		break;
	default:
		break;
	}
}


void Hit::DyingUpdate(float frametime)
{
	if (timer_ < DYING_DELAY)
	{
		timer_ += frametime;
		if (timer_ > DYING_DELAY)
		{
			Kill();
		}
		else
		{
			sf::Uint8 rate = (sf::Uint8) (255 * timer_ / DYING_DELAY);
			SetColor(sf::Color(255, 255, 255, 255 - rate));
			if (rotate_when_dying_)
			{
				Rotate(ANGLE_PER_SECOND * frametime);
			}
			else
			{
				speed_ *= SLOW_FACTOR;
				MoveLinear(frametime);
			}
		}
	}
}
