#include <iostream>

#include "WinInventory.hpp"
#include "../entities/Equipment.hpp"

#include "../gui_system/Button.hpp"

using namespace gui;
using namespace std;

WinInventory::WinInventory()
{
    /* Chargement du descriptif XML */
    Load("data/window/inventory.xml");

    last_item_ = 0;
    items_ = new Equipment*[ITEM_N+1];
}

WinInventory::~WinInventory()
{
    delete[] items_;

    UnLoad();
}

int WinInventory::AddItem(Equipment* item)
{
    if(last_item_ > ITEM_N){
        puts("inventaire plein");
        return 1;
    }
    Control *c = GetFromID(last_item_+ID_SLOT);
    if(c == NULL){
        puts("mauvaise id");
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
    cout << "Inventory callback [id:" << id << "]" << endl;
    if(id == ID_SLOT+14){
        return WinInventory::_CLOSE;
    }
    return 0;
}
