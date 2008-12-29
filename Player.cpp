#include "Player.hpp"
#include "MediaManager.hpp"
#include "Game.hpp"

#define SPEED    100


Player::Player(const sf::Vector2f& pos, const sf::Input& input) :
	Entity(pos, GET_IMG("player")),
	input_(input)
{
	// valeurs magiques (cf. bas de player.png)
	SetFloor((int) GetSize().x, 10);
	zone_ = NULL;
	
	move_keys_[UP] = sf::Key::Up;
	move_keys_[DOWN] = sf::Key::Down;
	move_keys_[LEFT] = sf::Key::Left;
	move_keys_[RIGHT] = sf::Key::Right;
}


void Player::Move(float frametime)
{
	static Game& game = Game::GetInstance();
	
	int dx, dy;
	sf::FloatRect rect;
	for (int dir = 0; dir < COUNT_DIRECTION; ++dir)
	{
		if (input_.IsKeyDown(move_keys_[dir]))
		{
			dx = dy = 0;
			switch (dir)
			{
				case UP:
					dy = -SPEED;
					break;
				case DOWN:
					dy = SPEED;
					break;
				case LEFT:
					dx = -SPEED;
					break;
				case RIGHT:
					dx = SPEED;
					break;
				default:
					break;
			}
			sf::Vector2f pos = GetPosition();
			pos.x += dx * frametime;
			pos.y += dy * frametime;
			
			rect.Left = pos.x;
			rect.Bottom = pos.y;
			rect.Right = pos.x + GetFloorWidth();
			rect.Top = pos.y - GetFloorHeight();
			
			// on vérifie si on doit changer de zone
			bool out_zone = false;
			if (rect.Left < 0)
			{
				game.ChangeZone(Game::LEFT);
				out_zone = true;
			}
			else if (rect.Top < 0)
			{
				game.ChangeZone(Game::UP);
				out_zone = true;
			}
			else if (rect.Right > Zone::WIDTH * Tile::SIZE)
			{
				game.ChangeZone(Game::RIGHT);
				out_zone = true;
			}
			else if (rect.Bottom > Zone::HEIGHT * Tile::SIZE)
			{
				game.ChangeZone(Game::DOWN);
				out_zone = true;
			}
	
			if (!out_zone && zone_->CanMove(rect))
			{
				SetPosition(pos);	
			}
		}
	}
}

