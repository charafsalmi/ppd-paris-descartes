#include "Equipment.hpp"
#include "EntityFactory.hpp"
#include "../gui/ControlPanel.hpp"


Equipment::Equipment(unsigned int code, const sf::Vector2f& position,
	const sf::IntRect& subrect) :
	Item(code, position, subrect)
{
}


void Equipment::OnCollide(Player& player)
{
	ControlPanel& panel = ControlPanel::GetInstance();
	(void) player;
	Equipment* clone = new Equipment(*this);
	panel.GetInventory()->AddItem(clone);
	std::string message = "equipement trouve : ";
	message += EntityFactory::GetInstance().GetItemName(GetTypeID());
	panel.PrintInfoText(message);
	Kill();
}
