#include <SFML/System.hpp>

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


bool Entity::Move(Direction dir, float frametime)
{
	//@TODO : à compléter
	(void) dir;
	(void) frametime;
	return false;
}


bool Entity::MoveUp(float frametime)
{
	return Move(UP, frametime);
}


bool Entity::MoveRight(float frametime)
{
	return Move(RIGHT, frametime);
}


bool Entity::MoveDown(float frametime)
{
	return Move(DOWN, frametime);
}


bool Entity::MoveLeft(float frametime)
{
	return Move(LEFT, frametime);
}


bool Entity::MoveRandomly(float frametime)
{
	return Move((Direction) sf::Randomizer::Random(0, COUNT_DIRECTION), frametime);
}


void Entity::SetActiveZone(Zone* zone)
{
	zone_ = zone;
}

