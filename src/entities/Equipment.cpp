#include "Equipment.hpp"
#include "Unit.hpp"
#include "EntityFactory.hpp"
#include "../core/Zone.hpp"
#include "../gui/ControlPanel.hpp"
#include "../misc/MediaManager.hpp"
#include "../misc/Die.hpp"


Equipment::Equipment(Item::Type type)
{
	SetImage(GET_IMG("items"));
	sf::IntRect rect;
	EntityFactory::GetInstance().GetItemSubRect(type, rect);
	SetSubRect(rect);

	switch (type)
	{
		case Item::ITM_BOW:
			ammo_ = 10;
			fire_rate_ = 1.f / 0.8f; // tirs par seconde
			break;
		case Item::ITM_SWORD:
			ammo_ = -1;
			fire_rate_ = 1.f / 0.5f;
			break;
		default:
			DIE("equipment code '%d' is not implemented", type);
			break;
	}
	type_ = type;
	last_used_ = 0;
	owner_ = NULL;
}


Item::Type Equipment::GetType() const
{
	return type_;
}


int Equipment::GetAmmo() const
{
	return ammo_;
}


void Equipment::AddAmmo(int amount)
{
	ammo_ += amount;
}


void Equipment::SetAmmo(int amount)
{
	ammo_ = amount;
}


void Equipment::SetOwner(Unit* owner)
{
	owner_ = owner;
}

