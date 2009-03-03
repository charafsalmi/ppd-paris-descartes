#ifndef ITEM
#define ITEM

#include <SFML/Graphics.hpp>

class Player;

/**
 * Base de tous les objets
 */
class Item: public sf::Sprite
{
public:
	Item(const sf::Vector2f& pos, const char* name);
	
	/**
	 * Méthode callback en cas de collision avec le joueur
	 * @param[in, out] player: joueur à modifier
	 */
	virtual void OnCollide(Player& player) = 0;
	
	/**
	 * Obtenir le rectangle de la surface de collision
	 */
	void GetRect(sf::FloatRect& rect) const;
	
	/**
	 * Nom générique de l'objet
	 */
	const std::string& GetName() const;
	
	inline bool IsDead() const
	{
		return dead_;
	}
	
protected:
	void Kill();
	
private:
	bool dead_;
	std::string name_;
};


class Money: public Item
{
public:
	Money(const sf::Vector2f& pos);
	
	void OnCollide(Player& player);
};


class Heart: public Item
{
public:
	Heart(const sf::Vector2f& pos);
	
	void OnCollide(Player& player);
};

#endif /* ITEM */

