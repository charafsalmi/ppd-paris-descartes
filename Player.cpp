#include "Player.hpp"
#include "MediaManager.hpp"
#include "Game.hpp"

#define SPEED    100


Player::Player(const sf::Vector2f& pos, Game& game) :
	Entity(pos, GET_IMG("player")),
	input_(game.GetInput())
{
	// valeurs magiques (cf. bas de player.png)
	SetFloor(GetSize().x, 10);
	zone_ = NULL;
}


void Player::SetZone(const Zone* zone)
{
	zone_ = zone;
}


void Player::Move(float frametime)
{
	static Game& game = Game::GetInstance();
	
	int dy = 0, dx = 0;
	if (input_.IsKeyDown(sf::Key::Up))
	{
		dy = -SPEED;
	}
	else if (input_.IsKeyDown(sf::Key::Down))
	{
		dy = SPEED;
	}
	
	if (input_.IsKeyDown(sf::Key::Left))
	{
		dx = -SPEED;
	}
	else if (input_.IsKeyDown(sf::Key::Right))
	{
		dx = SPEED;
	}
	// calcul de la prochaine position
	sf::Vector2f pos = GetPosition();
	pos.x += dx * frametime;
	pos.y += dy * frametime;
	
	sf::FloatRect rect;
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

