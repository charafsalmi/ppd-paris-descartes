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
	enum Direction
	{
		UP, DOWN, LEFT, RIGHT, COUNT_DIRECTION
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
	 * Calculer la distance entre deux entités
	 * @return distance en pixels
	 */
	float Distance(const Entity& entity) const;

	/**
	 * Déterminer si l'entité peut interragir avec le joueur
	 */
	virtual bool CanInteract() const;

	/**
	 * Déplacer l'entité selon un angle donné
	 * @param[in] angle
	 */
	//void MoveByAngle(float frametime, float speed, float angle);

	/**
	 * @brief Déplace l'entité au hasard, si possible
	 * @return true si déplacement est effectué
	 */
	//bool MoveRandomly(float frametime, float speed);

	/**
	 * Encaisser des dommages
	 * @param[in] damage: dommages infligés
	 */
	virtual void TakeDamage(int damage) = 0;

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
	 * Obtenir le rectangle à utiliser pour les déplacements
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
	 * Obtenir le rectangle à utiliser pour les collisions
	 */
	virtual inline void GetCollideRect(sf::FloatRect& rect) const
	{
		rect.Left = GetPosition().x;
		rect.Bottom = GetPosition().y;
		rect.Right = rect.Left + GetSize().x;
		rect.Top = rect.Bottom - GetSize().y;
	}

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
	// pour accéder rapidement à la zone active
	static Zone* zone_;

private:
	int id_; // identifiant unique de l'entité dans sa zone
	bool dead_;
	int floor_width_;
	int floor_height_;
};

#endif // ENTITY_HPP

