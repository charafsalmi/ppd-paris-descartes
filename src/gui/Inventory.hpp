#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <SFML/Graphics.hpp>


class Equipment;

class Inventory
{
public:
	Inventory();
	~Inventory();

	void OnEvent(sf::Key::Code code);

	void Show(sf::RenderWindow& app) const;

	void AddItem(Equipment* item);

private:
	enum
	{
		WIDTH = 6, HEIGHT = 4, SLOT_SIZE = 60
	};

	// TODO: remplacer les sprites par des InventoryObject
	sf::Sprite slots_[HEIGHT][WIDTH];
	Equipment* items_[HEIGHT][WIDTH];
	struct Cursor
	{
		sf::Sprite sprite;
		sf::Vector2i coords;
	};
	Cursor cursor_;
};


#endif /* INVENTORY_HPP */

