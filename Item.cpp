#include <iostream>

#include "Item.hpp"

#include "MediaManager.hpp"


Item::Item(sf::Vector2f& offset, ItemData& data) :
	ItemData(data)	
{
	dead_ = false;
	equipable_ = false;
	value_ = 1;
	obj_world_.SetImage(GET_IMG("objects"));
	obj_world_.SetCenter(0, obj_world_.GetImage()->GetHeight()); // coin bas-gauche
	//obj_inventory_.SetImage(GET_IMG(img_inventory_));

	floor_width_ = obj_world_.GetImage()->GetWidth();
	floor_height_ = obj_world_.GetImage()->GetHeight();
}


void Item::OnInterract_World()
{
	puts("Interract_World");
	std::cout << "\t" << dead_ << "\n";
}

void Item::Blit_World(sf::RenderTarget& screen)
{
	screen.Draw(obj_world_);
}

void Item::Blit_Inventory(sf::RenderTarget& screen, const sf::Vector2f& pos)
{
	obj_inventory_.SetPosition(pos);
	screen.Draw(obj_inventory_);
}	

int Item::Take()
{

	if (!dead_)
	{
		dead_ = true;
		return value_;
	}
	else
	{
		puts("Take sur un mort. Shouldn't happen");
		return 0;
	}
}

void Item::SetSubRect(char sprite, sf::IntRect& ir)
{
	if (sprite == 'w')
	{
		obj_world_.SetSubRect(ir);
	}

}

void Item::SetPosition(char sprite, sf::Vector2f& offset)
{
	if (sprite == 'w')
	{
		obj_world_.SetPosition(offset);
	}

}


