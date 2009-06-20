#include "EntityFactory.hpp"
#include "Mob.hpp"

#include "../core/Animation.hpp"
#include "../core/Tileset.hpp"
#include "../misc/MediaManager.hpp"
#include "../misc/Log.hpp"
#include "../misc/Die.hpp"
#include "../xml/tinyxml.h"


#include "Unit.hpp"
#include "Item.hpp"
#include "Decor.hpp"
#include "Equipment.hpp"

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
		DIE("can't open units definition %s\n%s", filename, doc.ErrorDesc());
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
			DIE("unit id is missing");
		}
		UnitPattern* unit = &units_[id];

		// name
		p = elem->Attribute("name");
		if (p == NULL)
		{
		    OutputD << UF_S << "L'unite " << id << " n'a pas d'attribut de nom" << lEnd;
			p = DEFAULT_NAME;
		}
		unit->name = p;

		// health points
		int hp;
		if (elem->QueryIntAttribute("hp", &hp) != TIXML_SUCCESS)
		{
			OutputD << UF_S << "L'unite " << id << " n'a pas d'attribut de hp" << lEnd;
			hp = DEFAULT_HP;
		}
		unit->hp = hp;

		// speed
		int speed;
		if (elem->QueryIntAttribute("speed", &speed) != TIXML_SUCCESS)
		{
		    OutputD << UF_S << "L'unite " << id << " n'a pas d'attribut de vitesse" << lEnd;
			speed = DEFAULT_SPEED;
		}
		unit->speed = speed;

		// animations
		p = elem->Attribute("animation");
		if (p == NULL)
		{
		    OutputD << UF_S << "L'unite " << id << " n'a pas d'attribut d'animation" << lEnd;
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

		// weapon (optional)
		p = elem->Attribute("weapon");
		// TODO: utiliser des strings pour identifier les objets, vérifier que la string est valide, ou NULL
		if (p != NULL)
		{
			unit->item = p;
		}
		else
		{
			unit->item = "NULL";
		}

		elem = elem->NextSiblingElement();
	}
}


void EntityFactory::LoadDecors(const char* filename)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(filename))
	{
		DIE("can't open decors definition %s\n%s", filename, doc.ErrorDesc());
	}

	TiXmlHandle handle(&doc);
	TiXmlElement* elem = handle.FirstChildElement().FirstChildElement().Element();
	int int_attr;
	while (elem != NULL)
	{
		// decor id
		if (elem->QueryIntAttribute("id", &int_attr) != TIXML_SUCCESS)
		{
		    DIE("decor id is missing");
		}
		DecorPattern* decor  = &decors_[int_attr];

		// position x
		if (elem->QueryIntAttribute("x", &int_attr) != TIXML_SUCCESS)
		{
			DIE("x attribute not found");
		}
		decor->x = int_attr;

		// position y
		if (elem->QueryIntAttribute("y", &int_attr) != TIXML_SUCCESS)
		{
			DIE("y attribute not found");
		}
		decor->y = int_attr;

		// width
		if (elem->QueryIntAttribute("w", &int_attr) != TIXML_SUCCESS)
		{
			DIE("width attribute not found");
		}
		decor->width = int_attr;

		// height
		if (elem->QueryIntAttribute("h", &int_attr) != TIXML_SUCCESS)
		{
			DIE("height attribute not found");
		}
		decor->height = int_attr;

		// nombre de tiles bloquantes
		if (elem->QueryIntAttribute("block", &int_attr) != TIXML_SUCCESS)
		{
			DIE("block attribute not found");
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
		mob->SetCenter(0, unit.anim[Entity::DOWN]->GetFrame(0).GetHeight());
		mob->ChooseDirection();
		Equipment* item = BuildEquipment(unit.item.c_str());
		mob->SetEquipment(item);
		return mob;
	}

	OutputE << UF_S << "Impossible de faire apparaitre le mob, mauvais id:" << id << lEnd;
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
	OutputE << UF_S << "Impossible de faire apparaitre le decor, mauvais id:" << id << lEnd;
	return NULL;
}


Item* EntityFactory::BuildItem(int id, const sf::Vector2f& position) const
{
	// TODO: définition en XML
	sf::IntRect subrect;
	switch (id)
	{
		case 1:
			subrect = sf::IntRect(0, 16, 0 + 16, 16 + 28);
			return new Item(id, position, subrect);
		case 2:
			subrect = sf::IntRect(0, 0, 16, 16);
			return new Item(id, position, subrect);
		case 10:
			subrect = sf::IntRect(16, 0, 16 + 18, 0 + 32);
			return new Equipment(id, position, subrect);
		case 11:
			subrect = sf::IntRect(35, 15, 35 + 32, 15 + 32);
			return new Equipment(id, position, subrect);
	}
	DIE("item id %s is not implemented", id);
	return NULL;
}


Equipment* EntityFactory::BuildEquipment(const char* name) const
{
	// TODO stocker les subrects dans une std::map<std::string, sf::IntRect>
	Equipment* item = NULL;
	if (strcmp(name, "bow") == 0)
	{
		sf::IntRect subrect = sf::IntRect(35, 15, 35 + 32, 15 + 32);
		item = new Equipment(11, sf::Vector2f(0, 0), subrect);
	}
	else if (strcmp(name, "NULL") != 0)
	{
		DIE("equipement \"%s\" is not implemented", name);
	}
	return item;
}


const char* EntityFactory::GetItemName(int id) const
{
	// TODO: définition en XML
	switch (id)
	{
		case 1:
			return "rubis";
		case 2:
			return "coeur de vie";
		case 10:
			return "epee";
		case 11:
			return "arc";
	}
	return "<no name>";
}

