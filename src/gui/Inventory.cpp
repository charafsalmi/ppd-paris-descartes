#include <sstream>

#include "Inventory.hpp"
#include "ControlPanel.hpp"
#include "../misc/MediaManager.hpp"
#include "../entities/Item.hpp"
#include "../entities/Equipment.hpp"
#include "../entities/EntityFactory.hpp"

#define ID_BUTTON_CLOSE 6000
#define SLOT_SIZE       60
#define OFFSET_X        20
#define OFFSET_Y        50
#define PADDX           15
#define PADDY           15

using namespace gui;


Inventory::Inventory()
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


Inventory::~Inventory()
{
	UnLoad();
	Clear();
}


bool Inventory::AddItem(Equipment* item)
{
	if (HasItem(item->GetType()))
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
	ControlPanel::GetInstance().PrintInfoText("L'inventaire est plein");
	return false;
}


bool Inventory::HasItem(Item::Type type)
{
	if (GetItem1Type() == type || GetItem2Type() == type || GetItem3Type() == type)
	{
		return true;
	}
	for (int i = 0; i < COUNT_H; ++i)
	{
		for (int j = 0; j < COUNT_W; ++j)
		{
			if (GetItemType(items_[i][j]) == type)
			{
				return true;
			}
		}
	}
	return false;
}


void Inventory::Clear()
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


int Inventory::WindowCallback(Control::ControlID id)
{
	return id == ID_BUTTON_CLOSE ? _CLOSE : 0;
}


void Inventory::Render(sf::RenderTarget& app) const
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


void Inventory::OnEvent(input::Action action)
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


void Inventory::PlaceItem(Equipment* item, int i, int j)
{
	items_[i][j] = item;
	if (item != NULL)
	{
		int x = (j * (SLOT_SIZE + PADDX)) + (SLOT_SIZE - item->GetSize().x) / 2;
		int y = (i * (SLOT_SIZE + PADDY)) + (SLOT_SIZE - item->GetSize().y) / 2;
		item->SetPosition(OFFSET_X + x, OFFSET_Y + y);
	}
}


void Inventory::SetItem1Type(int type)
{
	SetItem(item1_, type);
	ControlPanel::GetInstance().SetItem1(item1_);
}


int Inventory::GetItem1Type() const
{
	GetItemType(item1_);
}


void Inventory::SetItem2Type(int type)
{
	SetItem(item2_, type);
	ControlPanel::GetInstance().SetItem2(item2_);
}


int Inventory::GetItem2Type() const
{
	GetItemType(item2_);
}


void Inventory::SetItem3Type(int type)
{
	SetItem(item3_, type);
	ControlPanel::GetInstance().SetItem3(item3_);
}


int Inventory::GetItem3Type() const
{
	GetItemType(item3_);
}


void Inventory::SetItem(Equipment*& item, int inttype)
{
	if (inttype >= 0 && inttype < Item::COUNT_ITM )
	{
		item = new Equipment((Item::Type) inttype);
	}
}


int Inventory::GetItemType(const Equipment* item)
{
	return item == NULL ? Item::ITM_NONE : item->GetType();
}


std::string Inventory::StockToString() const
{
	std::ostringstream oss;
	for (int i = 0; i < COUNT_H; ++i)
	{
		for (int j = 0; j < COUNT_W; ++j)
		{
			oss << GetItemType(items_[i][j]);
			oss << ' ';
		}
	}
	return oss.str();
}


void Inventory::StockFromString(const std::string& str)
{
	std::istringstream iss(str);
	int type;
	for (int i = 0; i < COUNT_H; ++i)
	{
		for (int j = 0; j < COUNT_W; ++j)
		{
			if (iss >> type)
			{
				if (type < Item::COUNT_ITM)
				{
					SetItem(items_[i][j], (Item::Type) type);
					PlaceItem(items_[i][j], i, j);
				}
				else
				{
					printf("warning: item type '%d' is not a valid type\n", type);
				}
			}
			else
			{
				return;
			}
		}
	}
}


void Inventory::ClearItem(Equipment*& item)
{
	if (item != NULL)
	{
		delete item;
		item = NULL;
	}
}
