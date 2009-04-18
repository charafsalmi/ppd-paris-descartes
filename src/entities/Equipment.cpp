#include "Equipment.hpp"
#include "../gui/ControlPanel.hpp"


Equipment::Equipment(int type_id, const sf::Vector2f& position,
	const sf::IntRect& subrect) :
	Item(type_id, position, subrect)
{
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
