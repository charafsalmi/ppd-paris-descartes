#ifndef _GUARD_ITEM_HPP_
#define _GUARD_ITEM_HPP_

#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


/**
 *	Classe des objets du monde 
 *
 */

struct ItemData
{
	public:
		char *img_world_, *img_inventory_;
		std::string name_, desc_;
		
		ItemData()
		{
			img_world_ = img_inventory_ = NULL;
			uid_ = 0;
		}
		
		~ItemData()
		{
			if (img_world_ != NULL)
				delete[] img_world_;
			if (img_inventory_ != NULL)
				delete[] img_inventory_;
		}
	protected:
		mutable int uid_;		

};


class Item: public ItemData
{
	public:

		Item(sf::Vector2f& offset, ItemData& data);
		
		inline bool IsEquipable()
		{
			return equipable_;
		}

		// que se passe-t'il sur interraction avec l'objet, quand il est "activé" dans le monde
		// Ex: Ajouter l'objet à l'inventaire, déclencher un script...
		// Référence sur la zone fournie pour:
		// 	1. le cas ou un meme objet est instancié a différents endroits. [pas complet, faudrait référence sur le joueur]
		//  2. le cas ou l'objet agit sur le monde [sous réserve de possibilité de faire Zone::GetNext(Direction)]	
		virtual void OnInterract_World();
		
		// Blitte l'image de l'objet (destinée au monde)
		void Blit_World (sf::RenderTarget& app);
		
		// Blitte l'image de l'objet (destinée à l'inventaire), à pos
		void Blit_Inventory (sf::RenderTarget& app, const sf::Vector2f& pos);

		int Take();
		
		void SetSubRect(char sprite, sf::IntRect& ir);
		
		void SetPosition(char sprite, sf::Vector2f& offset);
		
		inline bool IsDead() const
		{
			return dead_;	
		}
		
		inline void GetFloorRect(sf::FloatRect& rect) const
		{
			const sf::Vector2f& pos = obj_world_.GetPosition();
			rect.Left = pos.x;
			rect.Bottom = pos.y;
			rect.Right = pos.x + floor_width_;
			rect.Top = pos.y - floor_height_;
		}
		
		/**
		 * @brief Comparaison de la profondeur de deux entités (axe y)
		 */
		inline bool operator<(const Item& other) const
		{
			return obj_world_.GetPosition().y < other.obj_world_.GetPosition().y;
		}

		/**
		 * @brief Comparaison de deux entités via des pointeurs
		 */
		inline static bool PtrComp(const Item* a, const Item* b)
		{
			return *a < *b;
		}
		
	protected:
		mutable sf::Sprite obj_world_;
		mutable sf::Sprite obj_inventory_;
		mutable bool equipable_;
	private:
		bool dead_;
		int value_;
		int floor_width_;
		int floor_height_;
};

#endif // _GUARD_ITEM_HPP_
