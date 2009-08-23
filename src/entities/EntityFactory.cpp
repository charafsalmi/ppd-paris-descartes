#include "EntityFactory.hpp"
#include "Mob.hpp"

#include "../core/Animation.hpp"
#include "../core/Tileset.hpp"
#include "../misc/MediaManager.hpp"
#include "../misc/Die.hpp"
#include "../xml/tinyxml.h"

#include "Unit.hpp"
#include "Item.hpp"
#include "Decor.hpp"
#include "Equipment.hpp"

#define UNIT_DEFINITION   "data/xml/units.xml"
#define DECOR_DEFINITION  "data/xml/decors.xml"
#define ITEM_DEFINITION   "data/xml/items.xml"

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
	Item::InitBehaviors();
	LoadItems(ITEM_DEFINITION);
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
			printf("warning: unit '%d' doesn't have 'name' attribute\n", id);
			p = DEFAULT_NAME;
		}
		unit->name = p;

		// health points
		int hp;
		if (elem->QueryIntAttribute("hp", &hp) != TIXML_SUCCESS)
		{
			printf("warning: unit '%d' doesn't have 'hp' attribute\n", id);
			hp = DEFAULT_HP;
		}
		unit->hp = hp;

		// speed
		int speed;
		if (elem->QueryIntAttribute("speed", &speed) != TIXML_SUCCESS)
		{
			printf("warning: unit '%d' doesn't have 'speed' attribute\n", id);
			speed = DEFAULT_SPEED;
		}
		unit->speed = speed;

		// animations
		p = elem->Attribute("animation");
		if (p == NULL)
		{
			printf("warning: unit '%d' doesn't have 'animation' attribute\n", id);
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
			unit->item.clear();
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

static void QueryInt(TiXmlElement* elem, const char* attribute, int& data)
{
	if (elem->QueryIntAttribute(attribute, &data) != TIXML_SUCCESS)
	{
		DIE("int attribute '%s' not found or invalid", attribute);
	}
}

static void QueryPtr(TiXmlElement* elem, const char* attribute, const char*& data)
{
	data = elem->Attribute(attribute);
	if (data == NULL)
	{
		DIE("string attribute '%s' not found", attribute);
	}
}

static void QueryStr(TiXmlElement* elem, const char* attribute, std::string& data)
{
	const char* p = NULL;
	QueryPtr(elem, attribute, p);
	data = p;
}



void EntityFactory::LoadItems(const char* filename)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(filename))
	{
		DIE("can't open items definition %s\n%s", filename, doc.ErrorDesc());
	}
	TiXmlHandle handle(&doc);
	TiXmlElement* elem = handle.FirstChildElement().FirstChildElement().Element();
	const char* name;
	while (elem != NULL)
	{
		QueryPtr(elem, "name", name);
		Item::Type item_type = Item::StringToType(name);

		ItemPattern item;

		QueryStr(elem, "label", item.label);
		QueryInt(elem, "x", item.x);
		QueryInt(elem, "y", item.y);
		QueryInt(elem, "w", item.w);
		QueryInt(elem, "h", item.h);

		items_[Item::StringToType(name)] = item;
		elem = elem->NextSiblingElement();
	}
}


Unit* EntityFactory::BuildUnit(int id, const sf::Vector2f& position) const
{
	UnitDef::const_iterator it = units_.find(id);
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
		if (!unit.item.empty())
		{
			Equipment* item = new Equipment(Item::StringToType(unit.item.c_str()));
			mob->SetEquipment(item);
		}
		return mob;
	}
	DIE("mob id %d is not implemented", id);
	return NULL;
}


Decor* EntityFactory::BuildDecor(int id, const sf::Vector2i& position) const
{
	DecorDef::const_iterator it = decors_.find(id);
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
	DIE("decor id %d is not implemented", id);
	return NULL;
}


Item* EntityFactory::BuildItem(Item::Type type, const sf::Vector2f& position) const
{
	ItemDef::const_iterator it = items_.find(type);
	if (it != items_.end())
	{
		const ItemPattern& item = it->second;
		sf::IntRect subrect;
		subrect.Top = item.y;
		subrect.Bottom = item.y + item.h;
		subrect.Left = item.x;
		subrect.Right = item.x + item.w;

		return new Item(type, position, subrect);
	}
	DIE("can't build item: item is not implemented");
	return NULL;
}


bool EntityFactory::GetItemSubRect(Item::Type type, sf::IntRect& subrect)
{
	ItemDef::const_iterator it = items_.find(type);
	if (it != items_.end())
	{
		const ItemPattern& item = it->second;
		subrect.Top = item.y;
		subrect.Bottom = item.y + item.h;
		subrect.Left = item.x;
		subrect.Right = item.x + item.w;
		return true;
	}
	puts("warning: can't get item subrect, item is not defined");
	return false;
}


const char* EntityFactory::GetItemName(Item::Type type) const
{
	ItemDef::const_iterator it = items_.find(type);
	if (it != items_.end())
	{
		return it->second.label.c_str();
	}
	puts("warning: can't get item name, item is not defined");
	return "<undefined>";
}

