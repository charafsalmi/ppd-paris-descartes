#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Animated.hpp"
#include "ControlPanel.hpp"
#include "Entity.hpp"

/**
 * Joueur contrôlable par l'utilisateur
 */
class Player: public Entity, public Animated
{
public:
	Player(const sf::Vector2f& pos, const sf::Input& input);
	
	/**
	 * Gérer un évènement clavier
	 * @param[in] key: touche pressée
	 */
	void OnEvent(sf::Key::Code key);

	void Update(float frametime);

	/**
	 * @brief Déplacer l'entité, si possible et en gérant le changement de zone
	 * @param[in] direction
	 * @param[in] frametime
	 * @return true si le déplacement a été effectué
	 */
	bool Move(Direction dir, float frametime);
	
private:
	/**
	 * Met à jour le subrect du joueur (uniquement si nécessaire)
	 * @param[in] dir: direction prise
	 * @param[in] moving: true si le joueur bouge
	 */
	void UpdateSubRect(Direction dir, bool moving = true);
		
	/**
	 * Détermine si deux directions forment une diagonale valide
	 * Par convention, si dir1 == dir2, renvoie false.
	 * @param[in] dir1
	 * @param[in] dir2
	 */
	 bool IsDiag(Direction dir1, Direction dir2);
	 
	// Keycodes des mouvements
	sf::Key::Code move_keys_[COUNT_DIRECTION];
	
	// Animations de déplacement
	const Animation* walk_anims_[COUNT_DIRECTION];
	
	// Subrects du sprite immobile
	sf::IntRect subrects_not_moving_[COUNT_DIRECTION];
	
	Direction current_dir_;
	const sf::Input& input_;
	ControlPanel& panel_;
	
	int lives_, rupees_;
};

#endif /* guard PLAYER_HPP */

