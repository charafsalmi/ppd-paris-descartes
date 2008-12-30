#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"

/**
 * Joueur contrôlable par l'utilisateur
 */
class Player: public Entity
{
	public:
		Player(const sf::Vector2f& pos, const sf::Input& input);
		
		/**
		 * Gérer un évènement
		 */
		void OnEvent(const sf::Event& event);
	
		void Move(float frametime);

		/**
		 * @brief Déplacer l'entité, si possible et en gérant le changement de zone
		 * @param[in] direction
		 * @param[in] frametime
		 * @return true si le déplacement a été effectué
		 */
		bool Move(Direction dir, float frametime);

		/**
		 * @brief Déplace l'entité vers le haut, si possible, tout en gérant les
		 * changement de zone
		 * @return true si déplacement est effectué
		 */
		//bool MoveUp(float frametime);

		/**
		 * @brief Déplace l'entité vers la droite, si possible, tout en gérant les
		 * changement de zone
		 * @return true si déplacement est effectué
		 */
		//bool MoveRight(float frametime);

		/**
		 * @brief Déplace l'entité vers le bas, si possible, tout en gérant les
		 * changement de zone
		 * @return true si déplacement est effectué
		 */
		//bool MoveDown(float frametime);

		/**
		 * @brief Déplace l'entité vers la gauche, si possible, tout en gérant les
		 * changement de zone
		 * @return true si déplacement est effectué
		 */
		//bool MoveLeft(float frametime);

		/**
		 * @brief Déplace l'entité au hasard, si possible, tout en gérant les
		 * changement de zone
		 * @return true si déplacement est effectué
		 */
		//bool MoveRandomly(float frametime);*/

	private:
		sf::Key::Code move_keys_[COUNT_DIRECTION];
		const sf::Input& input_;
};

#endif /* guard PLAYER_HPP */

