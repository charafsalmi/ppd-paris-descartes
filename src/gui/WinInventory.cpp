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
    item1_=0;
    item2_=0;
    item3_=0;
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
            case sf::Key::A:
                if(((WIDTH*y)+x)<last_item_){
                    if(items_[(WIDTH*y)+x]->GetTypeID()==item2_)item2_=item1_;
                    if(items_[(WIDTH*y)+x]->GetTypeID()==item3_)item3_=item1_;
                    item1_=items_[(WIDTH*y)+x]->GetTypeID();
                    cout << "[WinInventory]l'équipement d'ID " << item1_ << " à été associé au bouton A" << endl;
                }
                break;
            case sf::Key::Z:
                if(((WIDTH*y)+x)<last_item_){
                    if(items_[(WIDTH*y)+x]->GetTypeID()==item1_)item1_=item2_;
                    if(items_[(WIDTH*y)+x]->GetTypeID()==item3_)item3_=item2_;
                    item2_=items_[(WIDTH*y)+x]->GetTypeID();
                    cout << "[WinInventory]l'équipement d'ID " << item2_ << " à été associé au bouton Z" << endl;
                }
                break;
            case sf::Key::E:
                if(((WIDTH*y)+x)<last_item_){
                    if(items_[(WIDTH*y)+x]->GetTypeID()==item1_)item1_=item3_;
                    if(items_[(WIDTH*y)+x]->GetTypeID()==item2_)item2_=item3_;
                    item3_=items_[(WIDTH*y)+x]->GetTypeID();
                    cout << "[WinInventory]l'équipement d'ID " << item3_ << " à été associé au bouton E" << endl;
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
}

int WinInventory::GetItem1ID(){
    return item1_;
}

int WinInventory::GetItem2ID(){
    return item2_;
}

int WinInventory::GetItem3ID(){
    return item3_;
}

