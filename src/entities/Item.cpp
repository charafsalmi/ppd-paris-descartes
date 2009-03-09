#include "Item.hpp"
#include "Player.hpp"
#include "../misc/MediaManager.hpp"
#include "../gui/ControlPanel.hpp"


Item::Item(char code, const sf::Vector2f& position, const sf::IntRect& subrect) :
	Entity(position, GET_IMG("items"))
{
	SetSubRect(subrect);
	SetFloor(subrect.GetWidth(), subrect.GetHeight());
	SetCenter(0, subrect.GetHeight());

	code_ = code;
}


void Item::OnCollide(Player& player)
{
	static ControlPanel& panel = ControlPanel::GetInstance();
	switch (code_)
	{
		// money
		case 'M':
			player.AddMoney();
			panel.PrintInfoText(L"argent ramassé");
			Kill();
			break;
		// hearth
		case 'H':
			player.AddLife();
			panel.PrintInfoText(L"coeur ramassé");
			Kill();
			break;
	}
}

