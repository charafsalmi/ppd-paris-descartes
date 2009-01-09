#ifndef _GUARD_ITEM_HPP_
#define _GUARD_ITEM_HPP_

#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Zone.hpp"

/**
 *	Classe des objets du monde 
 *
 */

struct ItemData
{
	protected:
		mutable int uid_;		
		bool equipable_;
		char *img_world_, *img_inventory_;
		std::string name_, desc_;
};


class Item: public ItemData
{
	public:

		Item(ItemData& data);
		
		inline bool IsEquipable()
		{
			return equipable_;
		}

		// que se passe-t'il sur interraction avec l'objet, quand il est "activé" dans le monde
		// Ex: Ajouter l'objet à l'inventaire, déclencher un script...
		// Référence sur la zone fournie pour:
		// 	1. le cas ou un meme objet est instancié a différents endroits. [pas complet, faudrait référence sur le joueur]
		//  2. le cas ou l'objet agit sur le monde [sous réserve de possibilité de faire Zone::GetNext(Direction)]	
		virtual void OnInterract_World(Zone& zone);
		
		// Blitte l'image de l'objet (destinée au monde), à pos
		void Blit_World (sf::RenderTarget& app, const sf::Vector2f& pos);
		
		// Blitte l'image de l'objet (destinée à l'inventaire), à pos
		void Blit_Inventory (sf::RenderTarget& app, const sf::Vector2f& pos);

	protected:

		sf::Sprite obj_world_, obj_inventory_;
		
};

#endif // _GUARD_ITEM_HPP_
