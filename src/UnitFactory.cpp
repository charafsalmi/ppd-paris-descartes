#include "UnitFactory.hpp"
#include "MediaManager.hpp"
#include "Mob.hpp"
#include "tinyxml/tinyxml.h"

#define UNIT_DEFINITION "data/xml/units.xml"


UnitFactory& UnitFactory::GetInstance()
{
	static UnitFactory self;
	return self;
}


UnitFactory::UnitFactory()
{
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
		// id de l'unité
		int id;
		if (elem->QueryIntAttribute("id", &id) != TIXML_SUCCESS)
		{
			puts(" [UnitFactory] id attribute missing (skipped)");
			continue;
		}
		Pattern* pattern = &patterns_[id];
		
		// nom
		p = elem->Attribute("name");
		if (p == NULL)
		{
			puts(" [UnitFactory] name attribute missing (skipped)");
			continue;
		}
		pattern->name = p;
		
		// hp
		int hp;
		if (elem->QueryIntAttribute("hp", &hp) != TIXML_SUCCESS)
		{
			puts(" [UnitFactory] hp attribute missing (skipped)");
			continue;
		}
		pattern->hp = hp;
		
		// animations
		p = elem->Attribute("animation");
		if (p == NULL)
		{
			puts(" [UnitFactory] animation attribute missing (skipped)");
			continue;
		}
		pattern->image = &media.GetImage(p);
		std::string anim_name;
		
		anim_name = p;
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


Unit* UnitFactory::Make(int id, const sf::Vector2f& position)
{
	Definition::const_iterator it;
	it = patterns_.find(id);
	if (it != patterns_.end())
	{
		const Pattern& pattern = it->second;
		Mob* mob = new Mob(position, *pattern.image);
		mob->SetHP(pattern.hp);
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

