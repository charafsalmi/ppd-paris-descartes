#include "WinInventory.hpp"
#include "ControlPanel.hpp"
#include "../misc/Log.hpp"
#include "../misc/MediaManager.hpp"
#include "../entities/Equipment.hpp"

#define ID_BUTTON_CLOSE 6000
#define SLOT_SIZE       60
#define OFFSET_X        20
#define OFFSET_Y        50
#define PADDX           15
#define PADDY           15

using namespace gui;


WinInventory::WinInventory()
{
    // Chargement du descriptif XML
    Load("data/window/inventory.xml");

	for (int i = 0; i < COUNT_H; ++i)
	{
		for (int j = 0; j < COUNT_W; ++j)
		{
			items_[i][j] = NULL;
		}
	}

    cursor_.coords.x = 0;
	cursor_.coords.y = 0;
    cursor_.sprite.SetImage(GET_IMG("interface/inventory/cursor"));
    cursor_.sprite.SetPosition(OFFSET_X, OFFSET_Y);
	cursor_.sprite.SetColor(sf::Color(255, 255, 255));

    item1_ = NULL;
    item2_ = NULL;
    item3_ = NULL;
}


WinInventory::~WinInventory()
{
    UnLoad();
}


bool WinInventory::AddItem(Equipment* item)
{
	if (HasItem(item->GetTypeID()))
	{
		return false;
	}
	for (int i = 0; i < COUNT_H; ++i)
	{
		for (int j = 0; j < COUNT_W; ++j)
		{
			if (items_[i][j] == NULL)
			{
				PlaceItem(item, i, j);
				return true;
			}
		}
	}
	puts("inventaire plein");
	return false;
}


bool WinInventory::HasItem(int id)
{
	if (GetItem1ID() == id || GetItem2ID() == id || GetItem3ID() == id)
	{
		return true;
	}
	for (int i = 0; i < COUNT_H; ++i)
	{
		for (int j = 0; j < COUNT_W; ++j)
		{
			const Equipment* item = items_[i][j];
			if (item != NULL && item->GetTypeID() == id)
			{
				return true;
			}
		}
	}
	return false;
}


int WinInventory::WindowCallback(const Control::ControlID id, const int, void*)
{
	return id == ID_BUTTON_CLOSE ? _CLOSE : 0;
}


void WinInventory::Render(sf::RenderTarget& app) const
{
	Window::Render(app);
	app.Draw(cursor_.sprite);
	for (int i = 0; i < COUNT_H; ++i)
	{
		for (int j = 0; j < COUNT_W; ++j)
		{
			if (items_[i][j] != NULL)
			{
				app.Draw(*items_[i][j]);
			}
		}
	}
}


void WinInventory::OnEvent(input::Action action)
{
	static ControlPanel& panel = ControlPanel::GetInstance();
	int x = cursor_.coords.x;
	int y = cursor_.coords.y;
	bool valid = false;
	Equipment* temp = NULL;
	switch (action)
	{
		case input::MOVE_UP:
			valid = (--y >= 0);
			break;
		case input::MOVE_DOWN:
			valid = (++y < COUNT_H);
			break;
		case input::MOVE_LEFT:
			valid = (--x >= 0);
			break;
		case input::MOVE_RIGHT:
			valid = (++x < COUNT_W);
			break;
		case input::USE_ITEM_1:
			temp = item1_;
			panel.SetItem1Rect(items_[y][x] != NULL ? items_[y][x]->GetSubRect() : sf::IntRect(0, 0, 1, 1));
			item1_ = items_[y][x];
			PlaceItem(temp, y, x);
			break;
		case input::USE_ITEM_2:
			temp = item2_;
			panel.SetItem2Rect(items_[y][x] != NULL ? items_[y][x]->GetSubRect() : sf::IntRect(0, 0, 1, 1));
			item2_ = items_[y][x];
			PlaceItem(temp, y, x);
			break;
		case input::USE_ITEM_3:
			temp = item3_;
			panel.SetItem3Rect(items_[y][x] != NULL ? items_[y][x]->GetSubRect() : sf::IntRect(0, 0, 1, 1));
			item3_ = items_[y][x];
			PlaceItem(temp, y, x);
			break;
		default:
			break;
	}
	if (valid)
	{
		cursor_.coords.x = x;
		cursor_.coords.y = y;
		x = OFFSET_X + (x * (SLOT_SIZE + PADDX));
		y = OFFSET_Y + (y * (SLOT_SIZE + PADDY));
		cursor_.sprite.SetX(x);
		cursor_.sprite.SetY(y);
	}
}


void WinInventory::PlaceItem(Equipment* item, int i, int j)
{
	items_[i][j] = item;
	if (item != NULL)
	{
		int x = (j * (SLOT_SIZE + PADDX)) + (SLOT_SIZE - item->GetSize().x) / 2;
		int y = (i * (SLOT_SIZE + PADDY)) + (SLOT_SIZE - item->GetSize().y) / 2 + item->GetSize().y;
		item->SetPosition(OFFSET_X + x, OFFSET_Y + y);
	}
}


int WinInventory::GetItem1ID() const
{
	return item1_ == NULL ? -1 : item1_->GetTypeID();
}


int WinInventory::GetItem2ID() const
{
	return item2_ == NULL ? -1 : item2_->GetTypeID();
}


int WinInventory::GetItem3ID() const
{
	return item3_ == NULL ? -1 : item3_->GetTypeID();
}

