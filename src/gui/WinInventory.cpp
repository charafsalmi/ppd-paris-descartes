#include <iostream>

#include "WinInventory.hpp"
#include "../misc/MediaManager.hpp"
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
    cursor_.coords.x = 0;
	cursor_.coords.y = 0;
    cursor_.sprite.SetImage(GET_IMG("inventory-cursor"));
    cursor_.sprite.SetPosition(OFFSET_X, OFFSET_Y);
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

void WinInventory::Render(sf::RenderTarget& app) const{
    this->Window::Render(app);
    app.Draw(cursor_.sprite);
}

void WinInventory::OnEvent(const sf::Event& event)
{
    sf::Key::Code key = event.Key.Code;
	int x = cursor_.coords.x;
	int y = cursor_.coords.y;
	bool valid = false;
	if (event.Type == sf::Event::KeyPressed){
        switch (key)
        {
            case sf::Key::Up:
                valid = (--y >= 0);
                break;
            case sf::Key::Down:
                valid = (++y < HEIGHT);
                break;
            case sf::Key::Left:
                valid = (--x >= 0);
                break;
            case sf::Key::Right:
                valid = (++x < WIDTH);
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
}
