#include "Player.hpp"
#include "MediaManager.hpp"
#include "Game.hpp"
#include "type_definitions.hpp"

#define SPEED    100


Player::Player(const sf::Vector2f& pos, const sf::Input& input) :
	Entity(pos, GET_IMG("player")), input_(input)
{
	// valeurs magiques... surface de contact au sol
	SetFloor(28, 20);
	zone_ = NULL;
	
	// découpage du tileset du joueur
	subrects_[DOWN] = sf::IntRect(0, 0, 32, 48);
	subrects_[UP] = sf::IntRect(32, 0, 64, 48);
	subrects_[RIGHT] = sf::IntRect(64, 0, 96, 48);
	subrects_[LEFT] = sf::IntRect(96, 0, 128, 48);
	
	// attribution des touches de déplacement
	move_keys_[UP] = sf::Key::Up;
	move_keys_[DOWN] = sf::Key::Down;
	move_keys_[LEFT] = sf::Key::Left;
	move_keys_[RIGHT] = sf::Key::Right;
	
	// le joueur est de face par défaut
	current_dir_ = DOWN;
	SetSubRect(subrects_[DOWN]);
}


void Player::OnEvent(const sf::Event& event)
{
	if (event.Type == sf::Event::KeyPressed)
	{
		// gérer ici l'appui sur les touches
	}
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
			UpdateSubRect((Direction) dir);
			
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

			if (!out_zone && zone_->CanMove(this, rect))
			{
				SetPosition(pos);
			}
		}
	}
}


void Player::UpdateSubRect(Direction dir)
{
	if (dir != current_dir_)
	{
		current_dir_ = dir;
		SetSubRect(subrects_[dir]);
	}
}


bool Player::Move(Direction dir, float frametime)
{
	static Game& game = Game::GetInstance();

	SINT dx, dy;
	sf::FloatRect rect;

	dx = dy = 0;

	switch(dir)
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

	if(rect.Left < 0)
	{
		game.ChangeZone(Game::LEFT);
		out_zone = true;
	}
	else
		if(rect.Top < 0)
		{
			game.ChangeZone(Game::UP);
			out_zone = true;
		}
		else
			if(rect.Right > Zone::WIDTH * Tile::SIZE)
			{
				game.ChangeZone(Game::RIGHT);
				out_zone = true;
			}
			else
				if(rect.Bottom > Zone::HEIGHT * Tile::SIZE)
				{
					game.ChangeZone(Game::DOWN);
					out_zone = true;
				}

	if(!out_zone && zone_->CanMove(this, rect))
	{
		SetPosition(pos);
		return true;
	}
	return false;
}

