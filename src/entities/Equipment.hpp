#ifndef EQUIPMENT_HPP
#define EQUIPMENT_HPP

#include "Item.hpp"

#include "Unit.hpp"


class Equipment: public Item
{
public:
	Equipment(int type_id, const sf::Vector2f& position, const sf::IntRect& subrect);

	void OnCollide(Player& player);

	bool Use();

	int GetAmmo() const;

	void AddAmmo(int amount);

	void SetAmmo(int amount);

	void SetOwner(Unit* owner);

private:
	void UseBow();
	void UseSword();

	float fire_rate_;
	float last_used_;
	int ammo_;
	void (Equipment::*callback_)();
	Unit* owner_;
};

#endif // EQUIPMENT_HPP

