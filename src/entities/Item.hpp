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
	Item(int type_id_, const sf::Vector2f& pos, const sf::IntRect& subrect);

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
	 * Identifiant du type de l'objet
	 */
	int GetTypeID() const;

private:
	int type_id_; // id du type de l'item (!= id de l'item lui-même)
};

#endif /* ITEM_HPP */

