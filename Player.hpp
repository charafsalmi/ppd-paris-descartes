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
	
	void Move(float frametime);
	
private:
	enum Direction
	{
		UP, DOWN, LEFT, RIGHT, COUNT_DIRECTION
	};
	
	sf::Key::Code move_keys_[COUNT_DIRECTION];
	const sf::Input& input_;
};

#endif /* guard PLAYER_HPP */

