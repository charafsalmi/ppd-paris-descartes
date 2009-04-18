#ifndef PLAYERHIT_HPP
#define PLAYERHIT_HPP

#include "Hit.hpp"

/**
 * Coup émanant du joueur
 */
class PlayerHit: public Hit
{
public:
	PlayerHit(const sf::Vector2f position, int damage, Direction dir, int emitter_id_, Hit::Type type);

	/// inherited
	void OnCollide(Entity& entity, const sf::FloatRect& overlap);

private:

};


#endif /* PLAYERHIT_HPP */

