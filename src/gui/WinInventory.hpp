#ifndef MYWIN_HPP
#define MYWIN_HPP

#include "../gui_system/Window.hpp"

#define ID_SLOT 9000

class Equipment;

class WinInventory : public gui::Window{
public:
    WinInventory();

    void AddItem(Equipment* item);
private:
    int WindowCallback(const gui::Control::ControlID id, const int p1, void *p2);

	enum
	{
		WIDTH = 4, HEIGHT = 2, SLOT_SIZE = 60
	};

	Equipment* items_[HEIGHT][WIDTH];
};

#endif
