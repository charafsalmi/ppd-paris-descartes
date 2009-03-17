#include "Item.hpp"
#include "EntityFactory.hpp"
#include "Player.hpp"
#include "../misc/MediaManager.hpp"
#include "../gui/ControlPanel.hpp"


Item::Item(int type_id, const sf::Vector2f& position, const sf::IntRect& subrect) :
	Entity(position, GET_IMG("items"))
{
	SetSubRect(subrect);
	SetFloor(subrect.GetWidth(), subrect.GetHeight());
	SetCenter(0, subrect.GetHeight());

	type_id_ = type_id;
}


void Item::OnCollide(Player& player)
{
	static ControlPanel& panel = ControlPanel::GetInstance();
	switch (type_id_)
	{
		case 1:
			player.AddGold(1);
			break;
		case 2:
			player.AddHP();
			break;
	}
	std::string message = "trouve : ";
	message += EntityFactory::GetInstance().GetItemName(type_id_);
	panel.PrintInfoText(message);
	Kill();
}


int Item::GetTypeID() const
{
	return type_id_;
}
