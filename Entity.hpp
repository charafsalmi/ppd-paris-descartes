#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

class Zone;

/**
 * Base de toutes les entités
 */
class Entity: public sf::Sprite
{
public:
	/**
	 * @param[in] pos: position de l'entité (pixels)
	 * @param[in] image: image du sprite
	 */
	Entity(const sf::Vector2f& pos, const sf::Image& image);
	
	/**
	 * Déplacer l'entité
	 */
	virtual void Move(float frametime);
	
	/**
	 * Largeur du rectangle de contact avec le sol
	 */
	inline int GetFloorWidth() const
	{
		return floor_width_;
	}
	
	/**
	 * Hauteur du rectangle de contact avec le sol
	 */
	inline int GetFloorHeight() const
	{
		return floor_height_;
	}
	
	/**
	 * Comparaison de la profondeur de deux entités (axe y)
	 */
	inline bool operator<(const Entity& other) const
	{
		return GetPosition().y < other.GetPosition().y;
	}
	
	/**
	 * Comparaison de deux entités via des pointeurs
	 */
	inline static bool PtrComp(const Entity* a, const Entity* b)
	{
		return *a < *b;
	}
	
	/**
	 * Définit la zone active
	 */
	static void SetActiveZone(Zone* zone);
	
protected:
	/**
	 * Définir les dimensions du rectangle de contact avec le sol (pixels)
	 */
	inline void SetFloor(int w, int h)
	{
		floor_width_ = w;
		floor_height_ = h;
	}
	
	static Zone* zone_;
	
private:
	int floor_width_;
	int floor_height_;
};

#endif /* guard ENTITY_HPP */

