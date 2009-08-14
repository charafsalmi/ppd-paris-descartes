#include <sstream>

#include "WinInventory.hpp"
#include "ControlPanel.hpp"
#include "../misc/Log.hpp"
#include "../misc/MediaManager.hpp"
#include "../entities/Item.hpp"
#include "../entities/EntityFactory.hpp"

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
	Clear();
}


bool WinInventory::AddItem(Item* item)
{
	if (HasItem(item->GetTypeID()))
	{
		return false;
	}
	if (item1_ == NULL)
	{
		item1_ = item;
		ControlPanel::GetInstance().SetItem1(item1_);
		return true;
	}
	if (item2_ == NULL)
	{
		item2_ = item;
		ControlPanel::GetInstance().SetItem2(item2_);
		return true;
	}
	if (item3_ == NULL)
	{
		item3_ = item;
		ControlPanel::GetInstance().SetItem3(item3_);
		return true;
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
			const Item* item = items_[i][j];
			if (item != NULL && item->GetTypeID() == id)
			{
				return true;
			}
		}
	}
	return false;
}


void WinInventory::Clear()
{
	for (int i = 0; i < COUNT_H; ++i)
	{
		for (int j = 0; j < COUNT_W; ++j)
		{
			ClearItem(items_[i][j]);
		}
	}
	ClearItem(item1_);
	ClearItem(item2_);
	ClearItem(item3_);
}


int WinInventory::WindowCallback(Control::ControlID id)
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
	Item* temp = NULL;
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
			panel.SetItem1(items_[y][x]);
			item1_ = items_[y][x];
			PlaceItem(temp, y, x);
			break;
		case input::USE_ITEM_2:
			temp = item2_;
			panel.SetItem2(items_[y][x]);
			item2_ = items_[y][x];
			PlaceItem(temp, y, x);
			break;
		case input::USE_ITEM_3:
			temp = item3_;
			panel.SetItem3(items_[y][x]);
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


void WinInventory::PlaceItem(Item* item, int i, int j)
{
	items_[i][j] = item;
	if (item != NULL)
	{
		int x = (j * (SLOT_SIZE + PADDX)) + (SLOT_SIZE - item->GetSize().x) / 2;
		int y = (i * (SLOT_SIZE + PADDY)) + (SLOT_SIZE - item->GetSize().y) / 2 + item->GetSize().y;
		item->SetPosition(OFFSET_X + x, OFFSET_Y + y);
	}
}


void WinInventory::SetItem1ID(int id)
{
	SetItem(item1_, id);
	ControlPanel::GetInstance().SetItem1(item1_);
}


int WinInventory::GetItem1ID() const
{
	return item1_ == NULL ? -1 : item1_->GetTypeID();
}


void WinInventory::SetItem2ID(int id)
{
	SetItem(item2_, id);
	ControlPanel::GetInstance().SetItem2(item2_);
}


int WinInventory::GetItem2ID() const
{
	return item2_ == NULL ? -1 : item2_->GetTypeID();
}


void WinInventory::SetItem3ID(int id)
{
	SetItem(item3_, id);
	ControlPanel::GetInstance().SetItem3(item3_);
}


int WinInventory::GetItem3ID() const
{
	return item3_ == NULL ? -1 : item3_->GetTypeID();
}


std::string WinInventory::StockToString() const
{
	std::ostringstream oss;
	for (int i = 0; i < COUNT_H; ++i)
	{
		for (int j = 0; j < COUNT_W; ++j)
		{
			const Item* item = items_[i][j];
			oss << (item == NULL ? -1 : item->GetTypeID());
			oss << ' ';
		}
	}
	return oss.str();
}


void WinInventory::StockFromString(const std::string& str)
{
	std::istringstream iss(str);
	int id;
	for (int i = 0; i < COUNT_H; ++i)
	{
		for (int j = 0; j < COUNT_W; ++j)
		{
			if (iss >> id)
			{
				SetItem(items_[i][j], id);
				PlaceItem(items_[i][j], i, j);
			}
			else
			{
				return;
			}
		}
	}
}


void WinInventory::ClearItem(Item*& item)
{
	if (item != NULL)
	{
		delete item;
		item = NULL;
	}
}


void WinInventory::SetItem(Item*& item, int id)
{
	if (item == NULL && id != -1)
	{
		item = EntityFactory::GetInstance().BuildItem(id, sf::Vector2f(0, 0));
	}
}
