#ifndef WININVENTORY_HPP
#define WININVENTORY_HPP

#include "../gui_system/Window.hpp"

class Equipment;

class WinInventory : public gui::Window{
public:
    WinInventory();
    ~WinInventory();

    /** Retours de l'inventaire */
    enum
    {
        _CLOSE=101
    };

    /**
    Permet d'ajouter un item à l'inventaire
    @return 1 Si l'inventaire est plein
    */
    int AddItem(Equipment* item);

    /**
    * Permet de savoir si un item est deja présent dans l'inventaire
    * @return: true si il y est
    */
    bool HasItem(int id);

    void OnEvent(const sf::Event&);


private:
    void Render(sf::RenderTarget&) const;






    int WindowCallback(const gui::Control::ControlID id, const int p1, void *p2);

    /** Identifiant du slot principale */


    enum{
        ID_SLOT=9000, ITEM_N=15, WIDTH = 5, HEIGHT = 3, SLOT_SIZE = 60, OFFSET_X=20, OFFSET_Y=40, PADDX=15, PADDY=15
    };
    struct Cursor
	{
		sf::Sprite sprite;
		sf::Vector2i coords;
	};
	Cursor cursor_;
    /** Liste des équipements */
	Equipment** items_;
    int last_item_;
};

#endif
