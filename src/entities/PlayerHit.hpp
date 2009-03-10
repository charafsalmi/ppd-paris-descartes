#ifndef PLAYERHIT_HPP
#define PLAYERHIT_HPP

#include "Hit.hpp"

/**
 * Coup émanant du joueur
 */
class PlayerHit: public Hit
{
public:
	PlayerHit(const sf::Vector2f position, int damage, Direction dir);

	// inherited
	void OnCollide(Entity& entity);

private:

};


#endif /* PLAYERHIT_HPP */

