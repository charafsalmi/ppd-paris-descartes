#include <iostream>

#include "WinInventory.hpp"
#include "../entities/Equipment.hpp"

#define OFFSET_X ((640 - (WIDTH * SLOT_SIZE)) / 2.0)
#define OFFSET_Y ((480 - (HEIGHT * SLOT_SIZE)) / 2.0)

using namespace gui;
using namespace std;

WinInventory::WinInventory(){
    /* Chargement du descriptif XML */
    Load("data/window/inventory.xml");
}

void WinInventory::AddItem(Equipment* item){
    cout << "Add item to inventory" << endl;
}

int WinInventory::WindowCallback(const Control::ControlID id, const int p1, void *p2){
    cout << "Inventory callback" << endl;
    /*
    switch(id){
    }
    */
    return 0;
}
