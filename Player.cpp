#include "Player.hpp"
#include "MediaManager.hpp"
#include "Game.hpp"
#include "type_definitions.hpp"

#define SPEED    100
#define PL_WIDTH  32 
#define PL_HEIGHT 48


Player::Player(const sf::Vector2f& pos, const sf::Input& input) :
	Entity(pos, GET_IMG("player")),
	Animated(&GET_ANIM("player_walk_bottom"), *this),
	input_(input)
{
	// valeurs magiques... surface de contact au sol
	SetFloor(28, 20);
	zone_ = NULL;
	
	// Animations de marche
	walk_anims_[UP]		= &GET_ANIM("player_walk_top");
	walk_anims_[DOWN]	= &GET_ANIM("player_walk_bottom");
	walk_anims_[LEFT]	= &GET_ANIM("player_walk_left");
	walk_anims_[RIGHT]	= &GET_ANIM("player_walk_right");
	
	// Subrects d'immobilité
	subrects_not_moving_[UP]	= sf::IntRect(0,   0, 32,  48);
	subrects_not_moving_[DOWN]	= sf::IntRect(0,  96, 32, 144);
	subrects_not_moving_[RIGHT]	= sf::IntRect(0,  48, 32,  96);
	subrects_not_moving_[LEFT]	= sf::IntRect(0, 144, 32, 196);
	
	// attribution des touches de déplacement
	move_keys_[UP] = sf::Key::Up;
	move_keys_[DOWN] = sf::Key::Down;
	move_keys_[LEFT] = sf::Key::Left;
	move_keys_[RIGHT] = sf::Key::Right;
	
	// le joueur est de face par défaut
	current_dir_ = DOWN;
	SetSubRect(subrects_not_moving_[DOWN]);
	
	SetCenter(0, walk_anims_[UP]->GetFrame(0).GetHeight());
}


void Player::OnEvent(const sf::Event& event)
{
	if (event.Type == sf::Event::KeyPressed)
	{
		// gérer ici l'appui sur les touches
		if (event.Key.Code == sf::Key::Space)
		{
			std::cerr << " -- DEBUG -- \n";
			std::cout << "GetPosition: " << GetPosition().x << ", " << GetPosition().y << ";\n"; 
		}
	}
}


void Player::Update(float frametime)
{
	static Game& game = Game::GetInstance();
	bool moving = false;
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
					moving |= 1;
					break;
				case DOWN:
					dy = SPEED;
					moving |= 1;
					break;
				case LEFT:
					dx = -SPEED;
					moving |= 1;
					break;
				case RIGHT:
					dx = SPEED;
					moving |= 1;
					break;
				default:
					break;
			}
			UpdateSubRect((Direction) dir, moving);
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
	
	if (!moving)
	{
		Animated::Halt();
		SetSubRect(subrects_not_moving_[current_dir_]);
	}
	else
	{
		if (Animated::Halted())
		{
			std::cerr << "Restarting anim\n";
			Animated::Start();
		}
		Animated::Update(frametime, *this);
	}
}


void Player::UpdateSubRect(Direction dir, bool moving)
{
	if (dir != current_dir_)
	{	// Non remise à zero des animations sur mouvement diagonal.
		if (moving)
		{
		/*
			Si la touche précédemment appuyée ne l'est plus
			Ou que celle-ci et la nouvellement appuyée ne forment
			pas une diagonale, pas de changement d'anim.
		 */
			if (!(input_.IsKeyDown(move_keys_[current_dir_]) && IsDiag(dir, current_dir_)))
			{
				Animated::Change(walk_anims_[dir], *this);
				if (Animated::Halted())
				{
					Animated::Start();
				}
			}
		}
		else
		{
			Animated::Halt();
			SetSubRect(subrects_not_moving_[dir]);
		}
	current_dir_ = dir;
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
	else if(rect.Top < 0)
	{
		game.ChangeZone(Game::UP);
		out_zone = true;
	}
	else if(rect.Right > Zone::WIDTH * Tile::SIZE)
	{
		game.ChangeZone(Game::RIGHT);
		out_zone = true;
	}
	else if(rect.Bottom > Zone::HEIGHT * Tile::SIZE)
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


bool Player::IsDiag(Direction dir1, Direction dir2)
{
	bool res = true;
	
	if (dir1 == UP && dir2 == DOWN)
		res = false;
	else if (dir1 == LEFT && dir2 == RIGHT)
		res = false;
	else if (dir1 == DOWN && dir2 == UP)
		res = false;
	else if (dir1 == RIGHT && dir2 == LEFT)
		res = false;
	else if (dir1 == dir2)
		res = false;
	return res;
}

