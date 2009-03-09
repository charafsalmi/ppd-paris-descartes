#include "Equipment.hpp"
#include "../gui/ControlPanel.hpp"


Equipment::Equipment(char code, const sf::Vector2f& position,
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
	panel.PrintInfoText(L"Équipement trouvé");
	Kill();
}
