#include "WinInventory.hpp"
#include "ControlPanel.hpp"
#include "../misc/Log.hpp"
#include "../misc/MediaManager.hpp"
#include "../entities/Equipment.hpp"

using namespace gui;
using namespace std;

WinInventory::WinInventory()
{
    // Chargement du descriptif XML
    Load("data/window/inventory.xml");

    last_item_ = 0;
    items_ = new Equipment*[ITEM_N+1];

    cursor_.coords.x = 0;
	cursor_.coords.y = 0;
    cursor_.sprite.SetImage(GET_IMG("inventory-cursor"));
    cursor_.sprite.SetPosition(OFFSET_X, OFFSET_Y);
	cursor_.sprite.SetColor(sf::Color(255, 255, 255));

    item1_=-1;
    item2_=-1;
    item3_=-1;
}

WinInventory::~WinInventory()
{
    delete[] items_;

    UnLoad();
}

int WinInventory::AddItem(Equipment* item)
{
    if(last_item_ > ITEM_N){
        OutputD << "Inventaire plein" << lEnd;
        return 1;
    }
    Control *c = GetFromID(last_item_+ID_SLOT);
    if(c == NULL){
        OutputD << "Mauvaise id du control charge" << lEnd;
        return 0;
    }

    c->ChangeSprite(*item);
    items_[last_item_] = item;
    last_item_ ++;
}


bool WinInventory::HasItem(int id)
{
	for (int i = 0; i < last_item_; ++i)
	{
		if (items_[i]->GetTypeID() == id)
		{
			return true;
		}
	}
	return false;
}


int WinInventory::WindowCallback(const Control::ControlID id, const int p1, void *p2)
{
    OutputD << "Inventory callback [id:" << id << "]" << lEnd;
    if(id == ID_SLOT+14){
        return WinInventory::_CLOSE;
    }
    return 0;
}


void WinInventory::Render(sf::RenderTarget& app) const
{
    this->Window::Render(app);
    app.Draw(cursor_.sprite);
}


void WinInventory::OnEvent(input::Action action)
{
	static ControlPanel& panel = ControlPanel::GetInstance();
	int x = cursor_.coords.x;
	int y = cursor_.coords.y;
	bool valid = false;
	int current_item = -1;
	switch (action)
	{
		case input::MOVE_UP:
			valid = (--y >= 0);
			break;
		case input::MOVE_DOWN:
			valid = (++y < HEIGHT);
			break;
		case input::MOVE_LEFT:
			valid = (--x >= 0);
			break;
		case input::MOVE_RIGHT:
			valid = (++x < WIDTH);
			break;
		case input::USE_ITEM_1:
			// TODO factoriser ce truc avec un pointeur de méthode sur ControlPane:::SetItemXRect...
			current_item = WIDTH * y + x;
			if (current_item < last_item_)
			{
				if (current_item == item2_)
				{
					if (item1_ != -1)
						panel.SetItem2Rect(items_[item1_]->GetSubRect());
					else
						panel.SetItem2Rect(sf::IntRect(0, 0, 1, 1));
					item2_ = item1_;
				}
				if(current_item == item3_)
				{
					if (item1_ != -1)
						panel.SetItem3Rect(items_[item1_]->GetSubRect());
					else
						panel.SetItem3Rect(sf::IntRect(0,0,1,1));
					item3_=item1_;
				}
				item1_ = current_item;
				panel.SetItem1Rect(items_[item1_]->GetSubRect());

			}
			break;

		case input::USE_ITEM_2:
			if (((WIDTH*y)+x)<last_item_)
			{
				if ((WIDTH*y)+x==item1_)
				{
					if (item2_ != -1)
						panel.SetItem1Rect(items_[item2_]->GetSubRect());
					else
						panel.SetItem1Rect(sf::IntRect(0,0,1,1));
					item1_=item2_;
				}
				if ((WIDTH*y)+x==item3_)
				{
					if (item2_ != -1)
						panel.SetItem3Rect(items_[item2_]->GetSubRect());
					else
						panel.SetItem3Rect(sf::IntRect(0,0,1,1));
					item3_=item2_;
				}
				item2_=(WIDTH*y)+x;
				panel.SetItem2Rect(items_[item2_]->GetSubRect());

			}
			break;
		case input::USE_ITEM_3:
			if (((WIDTH*y)+x)<last_item_)
			{
				if ((WIDTH*y)+x==item1_)
				{
					if (item3_ != -1)
						panel.SetItem1Rect(items_[item3_]->GetSubRect());
					else
						panel.SetItem1Rect(sf::IntRect(0,0,1,1));
					item1_=item3_;
				}
				if ((WIDTH*y)+x==item2_)
				{
					if (item3_ != -1)
						panel.SetItem2Rect(items_[item3_]->GetSubRect());
					else
						panel.SetItem2Rect(sf::IntRect(0,0,1,1));
					item2_=item3_;
				}
				item3_=(WIDTH*y)+x;
				panel.SetItem3Rect(items_[item3_]->GetSubRect());

			}
			break;
		default:
			break;
	}
	if (valid)
	{
		cursor_.coords.x = x;
		cursor_.coords.y = y;
		cursor_.sprite.SetX(x * (SLOT_SIZE+PADDX) + OFFSET_X);
		cursor_.sprite.SetY(y * (SLOT_SIZE+PADDY) + OFFSET_Y);
	}
}


int WinInventory::GetItem1ID() const
{
	return item1_ != -1 ? items_[item1_]->GetTypeID() : 0;
}


int WinInventory::GetItem2ID() const
{
	return item2_ != -1 ? items_[item2_]->GetTypeID() : 0;
}


int WinInventory::GetItem3ID() const
{
	return item3_ != -1 ? items_[item3_]->GetTypeID() : 0;
}

