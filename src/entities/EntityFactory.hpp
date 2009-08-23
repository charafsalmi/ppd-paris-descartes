#ifndef ENTITYFACTORY_HPP
#define ENTITYFACTORY_HPP

#include <map>

#include "Entity.hpp"
#include "Item.hpp"

class Unit;
class Decor;
class Equipment;
class Animation;

/**
 * Construire des entités
 */
class EntityFactory
{
public:
	static EntityFactory& GetInstance();

	/**
	 * Allouer une unité
	 * @param[in] id: identifiant du type d'unité
	 * @param[in] position: emplacement en pixels
	 */
	Unit* BuildUnit(int id, const sf::Vector2f& position) const;

	/**
	 * Allouer un décor
	 * @param[in] id: identifiant du type de décor
	 * @param[in] position: emplacement en nombre de tiles
	 */
	Decor* BuildDecor(int id, const sf::Vector2i& position) const;

	/**
	 * Allouer un objet
	 * @param[in] id: identifiant du type d'objet
	 */
	Item* BuildItem(Item::Type type, const sf::Vector2f& position) const;

	/**
	 * Obtenir le nom d'un item
	 */
	const char* GetItemName(Item::Type type) const;

	/**
	 * Obtenir le sous rectangle d'un item dans sa feuille de sprite
	 * @return true si l'item est défini, sinon false
	 */
	bool GetItemSubRect(Item::Type type, sf::IntRect& subrect);

private:
	Equipment* BuildEquipment(const char* name) const;

	EntityFactory();
	EntityFactory(const EntityFactory& other);
	EntityFactory& operator=(const EntityFactory& other);

	void LoadUnits(const char* filename);
	void LoadDecors(const char* filename);
	void LoadItems(const char* filename);


	/**
	 * Profil d'une unité
	 */
	struct UnitPattern
	{
		int hp; // points de vie
		int speed; // vitesse (pixels/seconde)
		const Animation* anim[Entity::COUNT_DIRECTION];
		std::string name; // nom générique
		std::string item;
		const sf::Image* image;
	};

	// chaque profil est indexé par un id
	typedef std::map<int, UnitPattern> UnitDef;
	UnitDef units_;

	/**
	 * Profil d'un décor
	 */
	struct DecorPattern
	{
		// valeurs exprimées en nombre de tiles (et non en pixels)
		int x;
		int y;
		int width;
		int height;
		int block;
	};

	typedef std::map<int, DecorPattern> DecorDef;
	DecorDef decors_;

	/**
	 * Définition d'un item
	 */
	struct ItemPattern
	{
		std::string label;
		int x;
		int y;
		int w;
		int h;
	};
	typedef std::map<Item::Type, ItemPattern> ItemDef;
	ItemDef items_;
};

#endif // ENTITYFACTORY_HPP

