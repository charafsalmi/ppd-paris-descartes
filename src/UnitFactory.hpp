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
	
	struct Pattern
	{
		int hp;
		const Animation* anim[Entity::COUNT_DIRECTION];
		std::string name;
		const sf::Image* image;
	};
	
	typedef std::map<int, Pattern> Definition;
	Definition patterns_;
};


#endif /* guard UNITFACTORY_HPP */

