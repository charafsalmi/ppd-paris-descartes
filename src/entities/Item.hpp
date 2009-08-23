#ifndef ITEM_HPP
#define ITEM_HPP

#include <map>
#include <string>

#include "Entity.hpp"

class Player;

/**
 * Objet intéractif
 */
class Item: public Entity
{
public:
	enum Type
	{
		ITM_RUBY,
		ITM_HEART,
		ITM_SWORD,
		ITM_BOW,
		COUNT_ITM,
		ITM_NONE = -1
	};

	/**
	 * @param type: type de l'objet
	 * @param pos: position en pixels
	 * @param subrect: subrect du sprite
	 */
	Item(Type type, const sf::Vector2f& pos, const sf::IntRect& subrect);

	/**
	 * Initialiser les types
	 */
	static void InitBehaviors();

	/**
	 * Convertir une chaîne de caractère en type énuméré
	 * @return type énuméré Item::Type
	 */
	static Type StringToType(const char* name);

	static std::string TypeToString(Item::Type tyep);

	/**
	 * Méthode callback en cas de collision avec le joueur
	 * @param[in, out] player: joueur à modifier
	 */
	virtual void OnCollide(Player& player);

	// inherited
	void TakeDamage(int damage)
	{
		(void) damage;
	}

	/**
	 * Identifiant du type de l'objet
	 */
	int GetTypeID() const;

	/**
	 * Obtenir le nom de l'objet
	 */
	const char* GetName() const;
private:

	static std::map<std::string, Type> types_;
	Type my_type_;
};

#endif // ITEM_HPP

