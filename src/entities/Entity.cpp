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
