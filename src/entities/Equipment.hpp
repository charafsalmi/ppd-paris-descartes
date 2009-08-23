#ifndef EQUIPMENT_HPP
#define EQUIPMENT_HPP


#include "Item.hpp"
#include "Unit.hpp"

/**
 * Objet utilisable associé à un Item pour la représentation visuelle
 * Peut être utilisé par Unit, peut être stocké dans Inventory
 */
class Equipment: public sf::Sprite
{
public:
	Equipment(Item::Type type);

	Item::Type GetType() const;

	bool Use();

	int GetAmmo() const;

	void AddAmmo(int amount);

	void SetAmmo(int amount);

	void SetOwner(Unit* owner);

private:
	void UseBow();
	void UseSword();

	Item::Type type_;
	float fire_rate_;
	float last_used_;
	int ammo_;
	void (Equipment::*callback_)();
	Unit* owner_;
};

#endif // EQUIPMENT_HPP

