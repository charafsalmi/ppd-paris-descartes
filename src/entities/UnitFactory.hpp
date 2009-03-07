#ifndef UNITFACTORY_HPP
#define UNITFACTORY_HPP

#include <map>

#include "Unit.hpp"

/*
 * Usine à mobs
 */
class UnitFactory
{
public:
	static UnitFactory& GetInstance();

	Unit* Make(int id, const sf::Vector2f& position);

private:
	UnitFactory();
	UnitFactory(const UnitFactory& other);
	UnitFactory& operator=(const UnitFactory& other);

	/**
	 * Profil d'unité
	 */
	struct Pattern
	{
		int hp; // points de vie
		int speed; // vitesse (pixels/seconde)
		const Animation* anim[Entity::COUNT_DIRECTION];
		std::string name; // nom générique
		const sf::Image* image;
	};
	// chaque profil est indexé par un id
	typedef std::map<int, Pattern> Definition;
	Definition patterns_;
};


#endif /* guard UNITFACTORY_HPP */

