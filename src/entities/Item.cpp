#include "Item.hpp"
#include "Player.hpp"
#include "../misc/MediaManager.hpp"
#include "../gui/ControlPanel.hpp"


Item::Item(char code, const sf::Vector2f& position) :
	Entity(position, GET_IMG("items"))
{
	switch (code)
	{
		// money
		case 'M':
			SetSubRect(sf::IntRect(20, 0, 20 + 14, 0 + 28));
			SetFloor(14, 28);
			SetCenter(0, 28);
			break;
		// hearth
		case 'H':
			SetSubRect(sf::IntRect(2, 4, 2 + 14, 4 + 14));
			SetFloor(14, 14);
			SetCenter(0, 14);
			break;
		default:
			puts("bad item code");
			abort();
			break;
	}
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

