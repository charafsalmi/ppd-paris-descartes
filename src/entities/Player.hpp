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

	/// inherited
	void OnCollide(Entity& entity, const sf::FloatRect& overlap);

	/**
	 * Verrouiller le joueur (plus de mises à jour possibles)
	 */
	void Lock();

	/**
	 * Déverrouiller le joueur
	 */
	void Unlock();

	/**
	 * Ajouter un point de vie
	 */
	void AddHP();

	/**
	 * Incrémenter le compteur de frags
	 */
	void AddFrag();

	/**
	 * Augmenter l'or du joueur
	 */
	void AddGold(int amount = 1);

	/// inherited
	void Kill();

	/// inherited
	void TakeDamage(int damage);

	/// inherited
	void ThrowHit();
private:
	/**
	 * Convertir la position en pixels en position de tile
	 */
	void GetTilePosition(int& i, int& j);

	/// inherited
	void AutoUpdate(float frametime);

	void WalkUpdate(float frametime);

	void UseBowUpdate(float frametime);

	void FallingUpdate(float frametime);

	void (Player::*strategy_callback_)(float frametime);

    void UseItem(int);

	// Keycodes des mouvements
	sf::Key::Code move_keys_[COUNT_DIRECTION];

	const Animation* fall_anim_;

	// Subrects du sprite immobile
	sf::IntRect subrects_not_moving_[COUNT_DIRECTION];
	Direction current_dir_;
	bool was_moving_;
	bool locked_;
	const sf::Input& input_;
	ControlPanel& panel_;

	int max_lives_;
	int money_;
	int frags_;

	float started_action_;
	float falling_duration_;
	float use_bow_duration_;
	float last_hit_; // temps en secondes
};

#endif /* PLAYER_HPP */

