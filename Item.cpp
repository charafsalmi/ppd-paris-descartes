#include "Item.hpp"

#include "MediaManager.hpp"


Item::Item(ItemData& data) :
	ItemData(data)
{

	
	equipable_ = false;

	obj_world_.SetImage(GET_IMG(img_world_));
	obj_inventory_.SetImage(GET_IMG(img_inventory_));

}


void Item::OnInterract_World(Zone& zone)
{
	(void) zone;
	puts("Interract_World");
}

void Item::Blit_World(sf::RenderTarget& screen, const sf::Vector2f& pos)
{
	obj_world_.SetPosition(pos);
	screen.Draw(obj_world_);
}

void Item::Blit_Inventory(sf::RenderTarget& screen, const sf::Vector2f& pos)
{
	obj_inventory_.SetPosition(pos);
	screen.Draw(obj_inventory_);
}	
	

