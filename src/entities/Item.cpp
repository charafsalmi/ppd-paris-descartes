#include "Item.hpp"
#include "EntityFactory.hpp"
#include "Player.hpp"
#include "../core/SoundSystem.hpp"
#include "../misc/MediaManager.hpp"
#include "../gui/ControlPanel.hpp"
#include "../misc/Die.hpp"

#define ADD_BEHAVIOR(name) types_[#name] = name


std::map<std::string, Item::Type> Item::types_;


Item::Item(Type type, const sf::Vector2f& position, const sf::IntRect& subrect) :
	Entity(position, GET_IMG("items"))
{
	SetSubRect(subrect);
	SetFloor(subrect.GetWidth(), subrect.GetHeight());
	SetCenter(0, subrect.GetHeight());

	my_type_ = type;
}


Item::Type Item::StringToType(const char* name)
{
	if (name == NULL)
	{
		DIE("ptr NULL");
	}
	std::map<std::string, Type>::const_iterator it = types_.find(name);
	if (it == types_.end())
	{
		printf("error: item '%s' not defined\n", name);
	}
	return it->second;
}


std::string Item::TypeToString(Item::Type type)
{
	std::map<std::string, Item::Type>::const_iterator it;
	for (it = types_.begin(); it != types_.end(); ++it)
	{
		if (it->second == type)
		{
			return it->first;
		}
	}
	return "ITM_NONE";
}


void Item::InitBehaviors()
{
	ADD_BEHAVIOR(ITM_RUBY);
	ADD_BEHAVIOR(ITM_HEART);
	ADD_BEHAVIOR(ITM_BOW);
	ADD_BEHAVIOR(ITM_SWORD);
}


void Item::OnCollide(Player& player)
{
	static ControlPanel& panel = ControlPanel::GetInstance();
	switch (my_type_)
	{
		case ITM_RUBY:
			player.AddGold(1);
			break;

		case ITM_HEART:
			player.AddHP();
			break;

		case ITM_BOW:
		case ITM_SWORD:
			if (!player.AddEquipment(my_type_))
			{
				return;
			}
			break;

		default:
			break;
	}

	std::string message = "trouve : ";
	message += GetName();
	panel.PrintInfoText(message);
	SoundSystem::GetInstance().PlaySound("item-found");
	Kill();
}

const char* Item::GetName() const
{
	return EntityFactory::GetInstance().GetItemName(my_type_);
}

