#include "EntityFactory.hpp"
#include "Mob.hpp"
#include "Equipment.hpp"
#include "../misc/MediaManager.hpp"
#include "../xml/tinyxml.h"

#define UNIT_DEFINITION "data/xml/units.xml"

// profil par défaut (si incomplet)
#define DEFAULT_HP        3
#define DEFAULT_SPEED     80
#define DEFAULT_NAME      "Unamed entity"
#define DEFAULT_ANIMATION "Squelette"


EntityFactory& EntityFactory::GetInstance()
{
	static EntityFactory self;
	return self;
}


EntityFactory::EntityFactory()
{
	// loading units definition
	TiXmlDocument doc;
	if (!doc.LoadFile(UNIT_DEFINITION))
	{
		std::cerr << "can't open mob definitions: " << UNIT_DEFINITION << std::endl;
		abort();
	}
	TiXmlHandle handle(&doc);
	TiXmlElement* elem = handle.FirstChildElement().FirstChildElement().Element();

	const char* p = NULL;
	MediaManager& media = MediaManager::GetInstance();
	while (elem != NULL)
	{
		// unit id
		int id;
		if (elem->QueryIntAttribute("id", &id) != TIXML_SUCCESS)
		{
			puts(" [UnitFactory] id attribute missing");
			abort();
		}
		UnitPattern* pattern = &patterns_[id];

		// name
		p = elem->Attribute("name");
		if (p == NULL)
		{
#ifdef DEBUG
			printf("[UnitFactory] unit %d doesn't have name attribute\n", id);
#endif
			p = DEFAULT_NAME;
		}
		pattern->name = p;

		// health points
		int hp;
		if (elem->QueryIntAttribute("hp", &hp) != TIXML_SUCCESS)
		{
#ifdef DEBUG
			printf("[UnitFactory] unit %d doesn't have hp attribute\n", id);
#endif
			hp = DEFAULT_HP;
		}
		pattern->hp = hp;

		// speed
		int speed;
		if (elem->QueryIntAttribute("speed", &speed) != TIXML_SUCCESS)
		{
#ifdef DEBUG
			printf("[UnitFactory] unit %d doesn't have speed attribute\n", id);
#endif
			speed = DEFAULT_SPEED;
		}
		pattern->speed = speed;

		// animations
		p = elem->Attribute("animation");
		if (p == NULL)
		{
#ifdef DEBUG
			printf("[UnitFactory] unit %d doesn't have animation attribute\n", id);
#endif
			p = DEFAULT_ANIMATION;
		}
		pattern->image = &media.GetImage(p);

		std::string anim_name(p);
		anim_name += "_walk_up";
		pattern->anim[Entity::UP] = &media.GetAnimation(anim_name.c_str());

		anim_name = p;
		anim_name += "_walk_down";
		pattern->anim[Entity::DOWN] = &media.GetAnimation(anim_name.c_str());

		anim_name = p;
		anim_name += "_walk_left";
		pattern->anim[Entity::LEFT] = &media.GetAnimation(anim_name.c_str());

		anim_name = p;
		anim_name += "_walk_right";
		pattern->anim[Entity::RIGHT] = &media.GetAnimation(anim_name.c_str());

		printf("mob %s défini (id %d)\n", (pattern->name).c_str(), id);
		elem = elem->NextSiblingElement();
	}
}


Unit* EntityFactory::BuildUnit(int id, const sf::Vector2f& position) const
{
	Definition::const_iterator it;
	it = patterns_.find(id);
	if (it != patterns_.end())
	{
		const UnitPattern& pattern = it->second;
		Mob* mob = new Mob(position, *pattern.image, pattern.hp, pattern.speed);
		for (int i = 0; i < Entity::COUNT_DIRECTION; ++i)
		{
			mob->SetAnimation((Entity::Direction) i, pattern.anim[i]);
		}
		mob->Change(pattern.anim[Entity::DOWN], *mob);
		mob->SetCenter(0, pattern.anim[Entity::DOWN]->GetFrame(0).GetHeight());
		return mob;
	}
	std::cerr << "can't spawn mob, bad id: " << id << std::endl;
	return NULL;
}


Item* EntityFactory::BuildItem(char code, const sf::Vector2f& position) const
{
	sf::IntRect subrect;
	switch (code)
	{
		case 'M':
			subrect = sf::IntRect(0, 16, 0 + 16, 16 + 28);
			return new Item(code, position, subrect);
		case 'H':
			subrect = sf::IntRect(0, 0, 16, 16);
			return new Item(code, position, subrect);
		case 'S':
			subrect = sf::IntRect(16, 0, 16 + 18, 0 + 32);
			return new Equipment(code, position, subrect);
	}
	return NULL;
}