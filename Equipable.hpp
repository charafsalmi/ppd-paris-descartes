#ifndef _GUARD_EQUIPABLE_HPP_
#define _GUARD_EQUIPABLE_HPP_


#include "MediaManager.hpp"

#include "Item.hpp"

/**
 *	Classe des objets d'inventaire equipables (Epee, bombes, grappin...)
 *
 *
 */


class Equipable: public Item
{
	public:

		Equipable(ItemData& data);


	private:

};

#endif // _GUARD_EQUIPABLE_HPP_
