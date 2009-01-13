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
	 * Verrouiller le joueur (plus de mises à jour possibles)
	 */
	void Lock();
	
	/**
	 * Déverrouiller le joueur
	 */
	void Unlock();
	
private:
	// Keycodes des mouvements
	sf::Key::Code move_keys_[COUNT_DIRECTION];
	
	// Animations de déplacement
	const Animation* walk_anims_[COUNT_DIRECTION];
	const Animation* fall_anim_;
	
	// Subrects du sprite immobile
	sf::IntRect subrects_not_moving_[COUNT_DIRECTION];
	Direction current_dir_;
	bool was_moving_;
	bool locked_, falling_;
	const sf::Input& input_;
	ControlPanel& panel_;
	
	int lives_, rupees_;
};

#endif /* guard PLAYER_HPP */

