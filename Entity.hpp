#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include "type_definitions.hpp"
#include <ctime>

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
		 * @brief Déplacer l'entité
		 */
		virtual void Move(float frametime);

		/**
		 * @brief Déplacer l'entité, si possible et en gérant le changement de zone
		 * @param[in] direction
		 * @param[in] frametime
		 * @return true si le déplacement a été effectué
		 */
		virtual bool Move(CUSINT direction, float frametime);

		/**
		 * @brief Déplace l'entité vers le haut, si possible
		 * @return true si déplacement est effectué
		 */
		virtual bool MoveUp(float frametime);

		/**
		 * @brief Déplace l'entité vers la droite, si possible
		 * @return true si déplacement est effectué
		 */
		virtual bool MoveRight(float frametime);

		/**
		 * @brief Déplace l'entité vers le bas, si possible
		 * @return true si déplacement est effectué
		 */
		virtual bool MoveDown(float frametime);

		/**
		 * @brief Déplace l'entité vers la gauche, si possible
		 * @return true si déplacement est effectué
		 */
		virtual bool MoveLeft(float frametime);

		/**
		 * @brief Déplace l'entité au hasard, si possible
		 * @return true si déplacement est effectué
		 */
		virtual bool MoveRandomly(float frametime);

		/**
		 * @brief Largeur du rectangle de contact avec le sol
		 */
		inline int GetFloorWidth() const
		{
			return floor_width_;
		}

		/**
		 * @brief Hauteur du rectangle de contact avec le sol
		 */
		inline int GetFloorHeight() const
		{
			return floor_height_;
		}

		/**
		 * @brief Comparaison de la profondeur de deux entités (axe y)
		 */
		inline bool operator<(const Entity& other) const
		{
			return GetPosition().y < other.GetPosition().y;
		}

		/**
		 * @brief Comparaison de deux entités via des pointeurs
		 */
		inline static bool PtrComp(const Entity* a, const Entity* b)
		{
			return *a < *b;
		}

		/**
		 * @brief Définit la zone active
		 */
		static void SetActiveZone(Zone* zone);

	protected:
		enum Direction
		{
			UP, DOWN, LEFT, RIGHT, COUNT_DIRECTION
		};
		/**
		 * @brief Définir les dimensions du rectangle de contact avec le sol (pixels)
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

