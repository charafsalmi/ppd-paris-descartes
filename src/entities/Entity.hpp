#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

#include "../core/Tileset.hpp"

class Zone;

/**
 * Base de toutes les entités
 */
class Entity: public sf::Sprite
{
public:

	enum Direction
	{
		UP, DOWN, LEFT, RIGHT, COUNT_DIRECTION
	};

	/**
	 * Effet visuel lors de la collision de deux entités
	 */
	enum CollideEffect
	{
		FX_NOTING,
		FX_REJECTION,
		FX_STOP
	};

	/**
	 * @param[in] pos: position de l'entité (pixels)
	 * @param[in] image: image du sprite
	 */
	Entity(const sf::Vector2f& pos, const sf::Image& image);

	/**
	 * Mettre à jour l'entité
	 * @param[in] frametime: temps de la frame courante
	 */
	virtual void Update(float frametime);

	/**
	 * Callback de détection de collision
	 * @param[in, out] entity: entité entrée en collision
	 * @param[in] overlap: surface de la collision
	 */
	virtual void OnCollide(Entity& entity, const sf::FloatRect& overlap);

	/**
	 * Déterminer si l'entité peut interragir avec le joueur
	 */
	virtual bool CanInteract() const;

	/**
	 * Encaisser des dommages
	 * @param[in] damage: dommages infligés
	 */
	virtual void TakeDamage(int damage) = 0;

	/**
	 * @return effet visuel en cas de collision
	 */
	virtual CollideEffect GetCollideEffect() const;

	/**
	 * Copie polymorphique
	 * @return entité clonée
	 */
	//virtual Entity* Clone() const = 0;

	/**
	 * Calculer la distance entre deux entités
	 * @return distance en pixels
	 */
	float Distance(const Entity& entity) const;

	/**
	 * @brief Largeur du rectangle de contact avec le sol
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
	 * Obtenir le rectangle de la zone de contact avec le sol
	 * @param[out] rect: rectangle à définir
	 */
	inline void GetFloorRect(sf::FloatRect& rect) const
	{
		const sf::Vector2f& pos = GetPosition();
		rect.Left = pos.x;
		rect.Bottom = pos.y;
		rect.Right = pos.x + floor_width_;
		rect.Top = pos.y - floor_height_;
	}

	/**
	 * Obtenir le rectangle englobant tout l'entité
	 */
	inline void GetGlobalRect(sf::FloatRect& rect) const
	{
		rect.Left = GetPosition().x;
		rect.Bottom = GetPosition().y;
		rect.Right = rect.Left + GetSize().x;
		rect.Top = rect.Bottom - GetSize().y;
	}

	/**
	 * @return true si l'entité peut effectuer une collision avec
	 * le rectangle de contact au sol (floor)
	 */
	virtual bool CanFloorCollide() const;

	/**
	 * Comparer la profondeur de deux entités (axe y)
	 */
	inline bool operator<(const Entity& other) const
	{
		return GetPosition().y < other.GetPosition().y;
	}

	/**
	 * Comparer deux entités via des pointeurs
	 */
	inline static bool PtrComp(const Entity* a, const Entity* b)
	{
		return *a < *b;
	}

	/**
	 * Définir la zone active
	 */
	static void SetActiveZone(Zone* zone);

	/**
	 * Récupérer la zone active
	 */
	static Zone* GetActiveZone();

	/**
	 * Tuer l'objet entité (sera supprimé à frame n + 1)
	 */
	virtual void Kill();

	/**
	 * Détermine si l'unité doit être retirée de la zone
	 */
	inline bool IsDead() const
	{
		return dead_;
	}

	/**
	 * @return true si l'entité est sur le point d'être supprimée et doit être ignorée
	 */
	virtual bool IsDying() const;

	/**
	 * Définir les dimensions du rectangle de contact avec le sol
	 * @param[in] width: largeur en pixels
	 * @param[in] height: hauteur en pixels
	 */
	void SetFloor(int width, int height);

	/**
	 * Définir l'id de l'entité
	 */
	void SetID(int id);

	/**
	 * @return id de l'entité
	 */
	int GetID() const;

protected:
	/**
	 * Effectuer un déplacement orthogonal
	 * @param direction: sens du déplacement
	 * @param distance: distance en pixels
	 * @param tileflag: bitmask des tiles autorisées à franchir
	 * @return true si le déplacement a été effectué, sinon false
	 */
	bool Move(Direction direction, float distance, int tileflag = Tile::DEFAULT);

	// pour accéder rapidement à la zone active
	static Zone* zone_;

private:
	int id_; // identifiant unique de l'entité dans sa zone
	bool dead_;
	int floor_width_;
	int floor_height_;
};

#endif // ENTITY_HPP

