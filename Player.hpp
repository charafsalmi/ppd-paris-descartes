#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Zone.hpp"

class Game;

/**
 * Joueur contrôlable par l'utilisateur
 */
class Player: public Entity
{
public:
	Player(const sf::Vector2f& pos, Game& game);
	
	void Move(float frametime);
	
	/**
	 * Définir la zone active du joueur
	 */
	void SetZone(const Zone* zone);
	
private:
	enum Direction
	{
		UP, DOWN, LEFT, RIGHT, COUNT_DIRECTION
	};
	
	sf::Key::Code move_keys_[COUNT_DIRECTION];
	const sf::Input& input_;
	const Zone* zone_;
};

#endif /* guard PLAYER_HPP */

