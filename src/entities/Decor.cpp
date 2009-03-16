#include "Decor.hpp"


Decor::Decor(const sf::Vector2f& pos, const sf::Image& image) :
	Entity(pos, image)
{

}


void Decor::GetCollideRect(sf::FloatRect& rect) const
{
	// un élément du décor utilise toujours la surface de contact
	// avec le sol pour les collisions avec les entités
	GetFloorRect(rect);
}
