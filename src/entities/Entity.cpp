#include <cmath>
#include <SFML/System.hpp>

#include "Entity.hpp"
#include "../core/Zone.hpp"


Zone* Entity::zone_ = NULL;


Entity::Entity(const sf::Vector2f& pos, const sf::Image& image)
{
	SetImage(image);
	SetPosition(pos);
	floor_width_ = 0;
	floor_height_ = 0;
	dead_ = false;
	id_ = -1;
}


void Entity::Update(float)
{
}


void Entity::OnCollide(Entity&, const sf::FloatRect&)
{
}


float Entity::Distance(const Entity& entity) const
{
	float dx = GetPosition().x - entity.GetPosition().x;
	float dy = GetPosition().y - entity.GetPosition().y;

	return sqrt( (dx * dx) + (dy * dy) );
}


bool Entity::CanInteract() const
{
	return false;
}


Entity::CollideEffect Entity::GetCollideEffect() const
{
	return FX_NOTING;
}


bool Entity::CanFloorCollide() const
{
	return true;
}


void Entity::SetActiveZone(Zone* zone)
{
	zone_ = zone;
}


Zone* Entity::GetActiveZone()
{
	return zone_;
}


void Entity::Kill()
{
	dead_ = true;
}


bool Entity::IsDying() const
{
	return false;
}


void Entity::SetFloor(int width, int height)
{
	floor_width_ = width;
	floor_height_ = height;
}


void Entity::SetID(int id)
{
	id_ = id;
}


int Entity::GetID() const
{
	return id_;
}


bool Entity::Move(Direction direction, float distance, int tileflag)
{
	// construction du futur rectangle occupé
	sf::FloatRect rect;
	GetFloorRect(rect);
	switch (direction)
	{
		case UP:
			rect.Top -= distance;
			rect.Bottom -= distance;
			break;
		case DOWN:
			rect.Top += distance;
			rect.Bottom += distance;
			break;
		case LEFT:
			rect.Left -= distance;
			rect.Right -= distance;
			break;
		case RIGHT:
			rect.Left += distance;
			rect.Right += distance;
			break;
		default:
			break;
	}
	if (zone_->CanMove(rect, tileflag))
	{
		SetPosition(rect.Left, rect.Bottom);
		return true;
	}
	return false;
}
