#include "Item.hpp"
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
		// money
		case 1:
			player.AddMoney();
			panel.PrintInfoText("argent ramasse");
			Kill();
			break;
		// hearth
		case 2:
			player.AddLife();
			panel.PrintInfoText("coeur ramasse");
			Kill();
			break;
	}
}


int Item::GetTypeID() const
{
	return type_id_;
}
