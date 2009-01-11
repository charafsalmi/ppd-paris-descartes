#include "Inventory.hpp"
#include "MediaManager.hpp"

// origine de l'inventaire
#define OFFSET_X ((640 - (WIDTH * SLOT_SIZE)) / 2.0)
#define OFFSET_Y ((480 - (HEIGHT * SLOT_SIZE)) / 2.0)


Inventory::Inventory()
{
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			slots_[i][j].SetImage(GET_IMG("inventory-slot"));
			slots_[i][j].SetPosition(j * SLOT_SIZE + OFFSET_X, i * SLOT_SIZE + OFFSET_Y);
		}
	}
	cursor_.coords.x = 0;
	cursor_.coords.y = 0;
	cursor_.sprite.SetImage(GET_IMG("inventory-cursor"));
	cursor_.sprite.SetPosition(OFFSET_X, OFFSET_Y);
}


void Inventory::OnEvent(sf::Key::Code code)
{
	int x = cursor_.coords.x;
	int y = cursor_.coords.y;
	bool valid = false;
	switch (code)
	{
		case sf::Key::Up:
			if (--y >= 0)
			{
				valid = true;
			}
			break;
		case sf::Key::Down:
			if (++y < HEIGHT)
			{
				valid = true;
			}
			break;
		case sf::Key::Left:
			if (--x >= 0)
			{
				valid = true;
			}
			break;
		case sf::Key::Right:
			if (++x < WIDTH)
			{
				valid = true;
			}
			break;
		default:
			break;
	}
	if (valid)
	{
		cursor_.coords.x = x;
		cursor_.coords.y = y;
		cursor_.sprite.SetX(x * SLOT_SIZE + OFFSET_X);
		cursor_.sprite.SetY(y * SLOT_SIZE + OFFSET_Y);
	}
}


void Inventory::Show(sf::RenderWindow& app) const
{
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			app.Draw(slots_[i][j]);
		}
	}
	app.Draw(cursor_.sprite);
}

