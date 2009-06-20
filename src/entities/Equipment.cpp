#include "Equipment.hpp"
#include "Unit.hpp"
#include "Hit.hpp"

#include "../core/Game.hpp"
#include "../core/Zone.hpp"
#include "../gui/ControlPanel.hpp"


Equipment::Equipment(int type_id, const sf::Vector2f& position,
	const sf::IntRect& subrect) :
	Item(type_id, position, subrect)
{
	// TODO utiliser des strings pour identifier les objets utilisables
	if (type_id == 11)
	{
		ammo_ = 10;
		callback_ = &Equipment::UseBow;
		fire_rate_ = 1.f / 0.8f; // tirs par seconde
	}
	else if (type_id == 10)
	{
		ammo_ = -1;
		callback_ = &Equipment::UseSword;
		fire_rate_ = 1.f / 0.5f;
	}
	last_used_ = 0;
	owner_ = NULL;
}


void Equipment::OnCollide(Player& player)
{
	ControlPanel& panel = ControlPanel::GetInstance();

	if (!panel.GetInventory()->HasItem(GetTypeID()))
	{
		Equipment* clone = new Equipment(*this);
		panel.GetInventory()->AddItem(clone);
		Item::OnCollide(player);
	}
}


bool Equipment::Use()
{
	if (ammo_ != 0)
	{
		// délai minimum à respecter entre deux usages
		float now = Game::GetInstance().GetElapsedTime();
		if ((now - last_used_) > fire_rate_)
		{
			--ammo_;
			(this->*callback_)();
			last_used_ = now;
			return true;
		}
	}
	return false;
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


void Equipment::UseBow()
{
	sf::Vector2f pos;
	pos.x = owner_->GetPosition().x + owner_->GetSize().x / 2;
	pos.y = owner_->GetPosition().y - owner_->GetSize().y / 2;
	Hit* hit = new Hit(pos, 2, owner_->GetDirection(), owner_->GetID(), Hit::ARROW);
	Entity::GetActiveZone()->AddEntity(hit);
}


void Equipment::UseSword()
{
}

