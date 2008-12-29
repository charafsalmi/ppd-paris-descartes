#include "Entity.hpp"
#include "Zone.hpp"


Zone* Entity::zone_ = NULL;


Entity::Entity(const sf::Vector2f& pos, const sf::Image& image)
{
	SetImage(image);
	SetCenter(0, image.GetHeight()); // coin bas-gauche
	SetPosition(pos);
	floor_width_ = image.GetWidth();
	floor_height_ = image.GetHeight();
}


void Entity::Move(float frametime)
{
	(void) frametime;
}


void Entity::SetActiveZone(Zone* zone)
{
	zone_ = zone;
}

