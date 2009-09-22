#ifndef EQUIPMENT_HPP
#define EQUIPMENT_HPP


#include "Item.hpp"
#include "Unit.hpp"
#include "Hit.hpp"
#include "../core/Game.hpp"

/**
 * Objet utilisable associé à un Item pour la représentation visuelle
 * Peut être utilisé par Unit, peut être stocké dans Inventory
 */
class Equipment: public sf::Sprite
{
public:
	Equipment(Item::Type type);

	Item::Type GetType() const;

	/**
	 * Utiliser l'équipement
	 */
	template <class T>
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

template <class T>
bool Equipment::Use()
{
	if (ammo_ != 0)
	{
		// délai minimum à respecter entre deux usages
		float now = Game::GetInstance().GetElapsedTime();
		if ((now - last_used_) > fire_rate_)
		{
			sf::Vector2f pos = owner_->GetPosition();
			switch (type_)
			{
				case Item::ITM_BOW:
					pos.x = pos.x + owner_->GetSize().x / 2;
					pos.y = pos.y - owner_->GetSize().y / 2;
					Entity::GetActiveZone()->AddEntity(new T(pos, 2, owner_->GetDirection(), owner_->GetID(), Hit::ARROW));
					break;
				case Item::ITM_SWORD:
					puts("not implemented yet: sword");
					break;
				default:
					puts("shit happens");
					break;
			}
			if (ammo_ > 0)
			{
				--ammo_;
			}
			last_used_ = now;
			return true;
		}
	}
	return false;
}

#endif // EQUIPMENT_HPP

