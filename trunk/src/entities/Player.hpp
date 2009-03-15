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

	// inherited
	void OnCollide(Entity& entity);

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

	// inherited
	void Kill();

	// inherited
	void TakeDamage(int damage);

	//voir entity
	void ThrowHit();
private:
	// inherited
	void AutoUpdate(float frametime);

	void WalkUpdate(float frametime);

	void UseBowUpdate(float frametime);

	void FallingUpdate(float frametime);

	void (Player::*strategy_callback_)(float frametime);

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

	float started_action_;
	float falling_duration_;
	float use_bow_duration_;
	float last_hit_; // temps en secondes
};

#endif /* PLAYER_HPP */
