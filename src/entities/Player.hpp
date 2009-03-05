#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../gui/ControlPanel.hpp"
#include "Unit.hpp"

/**
 * Joueur contrôlable par l'utilisateur
 */
class Player: public Unit
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

	/**
	 * Ajouter une vie
	 */
	void AddLife();

	/**
	 * Augmenter l'argent d'une unité
	 */
	void AddMoney();

private:
	// Keycodes des mouvements
	sf::Key::Code move_keys_[COUNT_DIRECTION];

	const Animation* fall_anim_;

	// Subrects du sprite immobile
	sf::IntRect subrects_not_moving_[COUNT_DIRECTION];
	Direction current_dir_;
	bool was_moving_;
	bool locked_, falling_;
	const sf::Input& input_;
	ControlPanel& panel_;

	int max_lives_;
	int money_;
};

#endif /* PLAYER_HPP */

