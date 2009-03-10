#ifndef MYWIN_HPP
#define MYWIN_HPP

#include "../gui_system/Window.hpp"

class Equipment;

class WinInventory : public gui::Window{
public:
    WinInventory();
    ~WinInventory();

    /**
    Permet d'ajouter un item à l'inventaire
    @return 1 Si l'inventaire est plein
    */
    int AddItem(Equipment* item);
private:
    int WindowCallback(const gui::Control::ControlID id, const int p1, void *p2);

    /** Identifiant du slot principale */
    enum{
        ID_SLOT=9000, ITEM_N=8
    };

    /** Liste des équipements */
	Equipment** items_;
	int last_item_;
};

#endif
