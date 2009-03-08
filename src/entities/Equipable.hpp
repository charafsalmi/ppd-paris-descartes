#ifndef EQUIPABLE
#define EQUIPABLE

#include "Item.hpp"

/**
 *	Classe des objets d'inventaire équipables (Épée, bombes, grappin...)
 */
#if 0
class Equipable: public Item
{
public:
	Equipable(const sf::Vector2f& pos);

	void OnCollide(Player& player);

private:
	/*
	Alexandre:
	À mon avis ça ne suffit pas d'avoir un sprite de l'item tel qu'il est
	représenté dans l'inventaire.
	Un item équipable a beaucoup plus de caractéristiques
	(dégâts infligés, munitions, surface de collision, etc.)
	Peut-être faire une autre classe, et lier celle-ci par un pointeur
	dans l'item "sur le monde"
	*/
	sf::Sprite inventory_item_;
};
#endif

#endif /* EQUIPABLE */

