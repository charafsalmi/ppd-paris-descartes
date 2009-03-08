#ifndef ENTITYFACTORY_HPP
#define ENTITYFACTORY_HPP

#include <map>

#include "Unit.hpp"
#include "Item.hpp"

/**
 * Construire des entités
 */
class EntityFactory
{
public:
	static EntityFactory& GetInstance();

	Unit* BuildUnit(int id, const sf::Vector2f& position);

	Item* BuildItem(char code, const sf::Vector2f& position) const;

private:
	EntityFactory();
	EntityFactory(const EntityFactory& other);
	EntityFactory& operator=(const EntityFactory& other);

	/**
	 * Profil d'unité
	 */
	struct UnitPattern
	{
		int hp; // points de vie
		int speed; // vitesse (pixels/seconde)
		const Animation* anim[Entity::COUNT_DIRECTION];
		std::string name; // nom générique
		const sf::Image* image;
	};

	// chaque profil est indexé par un id
	typedef std::map<int, UnitPattern> Definition;
	Definition patterns_;
};


#endif /* ENTITYFACTORY_HPP */

