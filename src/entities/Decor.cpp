#include <typeinfo>

#include "Decor.hpp"
#include "Player.hpp"
#include "../core/Tileset.hpp"
#include "../core/InputController.hpp"

#define PUSH_DELAY 1.0f
#define MOVE_DELAY 2.0f


Decor::Decor(const sf::Vector2f& pos, const sf::Image& image) :
	Entity(pos, image)
{
	status_ = MOVABLE;
	pusher_ = NULL;
	push_direction_ = UP;
	timer_ = 0;
}


void Decor::SetMovable(bool movable)
{
	status_ = movable ? MOVABLE : NOT_MOVABLE;
}


Entity* Decor::Clone() const
{
	return new Decor(*this);
}


void Decor::Update(float frametime)
{
	switch (status_)
	{
		case PUSHED:
			{
				input::Action action;
				switch (push_direction_)
				{
					case UP:
						action = input::MOVE_UP;
						break;
					case DOWN:
						action = input::MOVE_DOWN;
						break;
					case LEFT:
						action = input::MOVE_LEFT;
						break;
					case RIGHT:
						action = input::MOVE_RIGHT;
						break;
				}
				if (InputController::GetInstance().HasInput(action))
				{
					timer_ -= frametime;
					if (timer_ <= 0)
					{
						status_ = MOVING;
						timer_ = MOVE_DELAY;
					}
					break;
				}
			}
			status_ = MOVABLE;
			pusher_ = NULL;
			timer_ = 0;
			break;
		case MOVING:
			if (Entity::Move(push_direction_,
				Tile::SIZE * frametime / MOVE_DELAY, Tile::DEFAULT))
			{
				timer_ -= frametime;
				if (timer_ > 0)
				{
					break;
				}
			}
			// déplacement terminé, ou déplacement impossible
			status_ = MOVABLE;
			timer_ = 0;
			break;
		default:
			break;
	}
}


Entity::CollideEffect Decor::GetCollideEffect() const
{
	return FX_STOP;
}


void Decor::OnCollide(Entity& entity, const sf::FloatRect& overlap)
{
	if (status_ == MOVABLE)
	{
		Player* player = dynamic_cast<Player*>(&entity);
		if (player != NULL)
		{
			status_ = PUSHED;
			push_direction_ = player->GetDirection();
			timer_ = PUSH_DELAY;
		}
	}
	else if (status_ == MOVING && typeid (entity) == typeid (Decor))
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
		Entity::Move(dir, dist);
	}
}


void Decor::GetCollideRect(sf::FloatRect& rect) const
{
	// un élément du décor utilise toujours la surface de contact
	// avec le sol pour les collisions avec les entités
	GetFloorRect(rect);
}
