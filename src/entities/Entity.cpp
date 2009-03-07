#include <SFML/System.hpp>

#include "Entity.hpp"
#include "../core/Zone.hpp"


Zone* Entity::zone_ = NULL;


Entity::Entity(const sf::Vector2f& pos, const sf::Image& image)
{
	SetImage(image);
	SetCenter(0, image.GetHeight()); // coin bas-gauche
	SetPosition(pos);
	floor_width_ = image.GetWidth();
	floor_height_ = image.GetHeight();
	dead_ = false;
}


void Entity::Update(float frametime)
{
	(void) frametime;
}


void Entity::OnCollide(Entity& entity)
{
	(void) entity;
}


void Entity::SetActiveZone(Zone* zone)
{
	zone_ = zone;
}


/*bool Entity::Move(Direction dir, float frametime)
{
	static const int MAGIC = 42;// vitesse
	int dx, dy;
	switch (dir)
	{
		case UP:
			dy = -MAGIC * frametime;
			break;
		case DOWN:
			dy = MAGIC * frametime;
			break;
		case LEFT:
			dx = -MAGIC * frametime;
			break;
		case RIGHT:
			dx = MAGIC * frametime;
			break;
	}
	Move(dx, dy);
	return false;
}*/


/*bool Entity::MoveRandomly(float frametime)
{
	return Move((Direction) sf::Randomizer::Random(0, COUNT_DIRECTION),
		frametime);
}*/


void Entity::Kill()
{
	dead_ = true;
}


void Entity::SetFloor(int width, int height)
{
	floor_width_ = width;
	floor_height_ = height;
}

