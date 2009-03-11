#include "EntityFactory.hpp"
#include "Mob.hpp"
#include "Equipment.hpp"
#include "../core/Tileset.hpp"
#include "../misc/MediaManager.hpp"
#include "../xml/tinyxml.h"

#define UNIT_DEFINITION   "data/xml/units.xml"
#define DECOR_DEFINITION  "data/xml/decors.xml"

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
	LoadUnits(UNIT_DEFINITION);
	LoadDecors(DECOR_DEFINITION);
}


void EntityFactory::LoadUnits(const char* filename)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(filename))
	{
		std::cerr << "can't open mob definitions: " << filename << std::endl;
		abort();
	}
	TiXmlHandle handle(&doc);

	const char* p = NULL;
	MediaManager& media = MediaManager::GetInstance();

	TiXmlElement* elem = handle.FirstChildElement().FirstChildElement().Element();
	while (elem != NULL)
	{
		// unit id
		int id;
		if (elem->QueryIntAttribute("id", &id) != TIXML_SUCCESS)
		{
			puts(" [UnitFactory] unit id missing");
			abort();
		}
		UnitPattern* unit = &units_[id];

		// name
		p = elem->Attribute("name");
		if (p == NULL)
		{
#ifdef DEBUG
			printf("[UnitFactory] unit %d doesn't have name attribute\n", id);
#endif
			p = DEFAULT_NAME;
		}
		unit->name = p;

		// health points
		int hp;
		if (elem->QueryIntAttribute("hp", &hp) != TIXML_SUCCESS)
		{
#ifdef DEBUG
			printf("[UnitFactory] unit %d doesn't have hp attribute\n", id);
#endif
			hp = DEFAULT_HP;
		}
		unit->hp = hp;

		// speed
		int speed;
		if (elem->QueryIntAttribute("speed", &speed) != TIXML_SUCCESS)
		{
#ifdef DEBUG
			printf("[UnitFactory] unit %d doesn't have speed attribute\n", id);
#endif
			speed = DEFAULT_SPEED;
		}
		unit->speed = speed;

		// animations
		p = elem->Attribute("animation");
		if (p == NULL)
		{
#ifdef DEBUG
			printf("[UnitFactory] unit %d doesn't have animation attribute\n", id);
#endif
			p = DEFAULT_ANIMATION;
		}
		unit->image = &media.GetImage(p);

		std::string anim_name(p);
		anim_name += "_walk_up";
		unit->anim[Entity::UP] = &media.GetAnimation(anim_name.c_str());

		anim_name = p;
		anim_name += "_walk_down";
		unit->anim[Entity::DOWN] = &media.GetAnimation(anim_name.c_str());

		anim_name = p;
		anim_name += "_walk_left";
		unit->anim[Entity::LEFT] = &media.GetAnimation(anim_name.c_str());

		anim_name = p;
		anim_name += "_walk_right";
		unit->anim[Entity::RIGHT] = &media.GetAnimation(anim_name.c_str());

		printf("mob %s défini (id %d)\n", (unit->name).c_str(), id);
		elem = elem->NextSiblingElement();
	}
}


void EntityFactory::LoadDecors(const char* filename)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(filename))
	{
		std::cerr << "can't open decor definitions: " << filename << std::endl;
		std::cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;
		abort();
	}
	MediaManager& media = MediaManager::GetInstance();

	TiXmlHandle handle(&doc);
	TiXmlElement* elem = handle.FirstChildElement().FirstChildElement().Element();
	int int_attr;
	while (elem != NULL)
	{
		// decor id
		if (elem->QueryIntAttribute("id", &int_attr) != TIXML_SUCCESS)
		{
			puts(" [UnitFactory] decor id missing");
			abort();
		}
		DecorPattern* decor  = &decors_[int_attr];

		// position x
		if (elem->QueryIntAttribute("x", &int_attr) != TIXML_SUCCESS)
		{
			abort();
		}
		decor->x = int_attr;

		// position y
		if (elem->QueryIntAttribute("y", &int_attr) != TIXML_SUCCESS)
		{
			abort();
		}
		decor->y = int_attr;

		// width
		if (elem->QueryIntAttribute("w", &int_attr) != TIXML_SUCCESS)
		{
			abort();
		}
		decor->width = int_attr;

		// height
		if (elem->QueryIntAttribute("h", &int_attr) != TIXML_SUCCESS)
		{
			abort();
		}
		decor->height = int_attr;

		// nombre de tiles bloquantes
		if (elem->QueryIntAttribute("block", &int_attr) != TIXML_SUCCESS)
		{
			abort();
		}
		decor->block = int_attr;
		elem = elem->NextSiblingElement();
	}
}


Unit* EntityFactory::BuildUnit(int id, const sf::Vector2f& position) const
{
	UnitDef::const_iterator it;
	it = units_.find(id);
	if (it != units_.end())
	{
		const UnitPattern& unit = it->second;
		Mob* mob = new Mob(position, *unit.image, unit.hp, unit.speed);
		for (int i = 0; i < Entity::COUNT_DIRECTION; ++i)
		{
			mob->SetAnimation((Entity::Direction) i, unit.anim[i]);
		}
		mob->Change(unit.anim[Entity::DOWN], *mob);
		mob->SetCenter(0, unit.anim[Entity::DOWN]->GetFrame(0).GetHeight());
		return mob;
	}
	std::cerr << "can't spawn mob, bad id: " << id << std::endl;
	return NULL;
}


Decor* EntityFactory::BuildDecor(int id, const sf::Vector2i& position) const
{
	DecorDef::const_iterator it;
	it = decors_.find(id);
	if (it != decors_.end())
	{
		const DecorPattern& decor_p = it->second;
		sf::Vector2f real_pos;
		real_pos.x = position.x * Tile::SIZE;
		real_pos.y = position.y * Tile::SIZE;

		Decor* decor = new Decor(real_pos, GET_IMG("decors"));
		sf::IntRect subrect;
		subrect.Top = decor_p.y * Tile::SIZE;
		subrect.Left = decor_p.x * Tile::SIZE;
		subrect.Bottom = subrect.Top + (decor_p.height * Tile::SIZE);
		subrect.Right = subrect.Left + (decor_p.width * Tile::SIZE);
		decor->SetSubRect(subrect);
		decor->SetCenter(0, subrect.GetHeight());
		decor->SetFloor(subrect.GetWidth(), decor_p.block * Tile::SIZE);
		return decor;
	}
	std::cerr << "can't spawn decor, bad id: " << id << std::endl;
	return NULL;
}


Item* EntityFactory::BuildItem(unsigned int code, const sf::Vector2f& position) const
{
	sf::IntRect subrect;
	switch (code)
	{
		case 01:
			subrect = sf::IntRect(0, 16, 0 + 16, 16 + 28);
			return new Item(code, position, subrect);
		case 02:
			subrect = sf::IntRect(0, 0, 16, 16);
			return new Item(code, position, subrect);
		case 10:
			subrect = sf::IntRect(16, 0, 16 + 18, 0 + 32);
			return new Equipment(code, position, subrect);
	}
	return NULL;
}
