#ifndef ITEM_HPP
#define ITEM_HPP

#include "Entity.hpp"

class Player;

/**
 * Objet intéractif
 */
class Item: public Entity
{
public:
	Item(unsigned int code, const sf::Vector2f& pos, const sf::IntRect& subrect);

	/**
	 * Méthode callback en cas de collision avec le joueur
	 * @param[in, out] player: joueur à modifier
	 */
	virtual void OnCollide(Player& player);

	// inherited
	void TakeDamage(int damage)
	{
		(void) damage;
	}

	/**
	 * Nom générique de l'objet
	 */
	//const char* GetName() const;

private:
	unsigned int code_;
};

#endif /* ITEM_HPP */

