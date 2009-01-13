#include <iostream>
#include <cstring>

#include "Player.hpp"
#include "Game.hpp"
#include "MediaManager.hpp"
#include "Tileset.hpp"
#include "Zone.hpp"

#define SPEED      100
#define FALL_DELAY   1


Player::Player(const sf::Vector2f& pos, const sf::Input& input) :
	Entity(pos, GET_IMG("player")),
	Animated(&GET_ANIM("player_walk_bottom"), *this),
	input_(input),
	panel_(ControlPanel::GetInstance())
{
	// valeurs magiques... surface de contact au sol
	SetFloor(28, 20);
	
	// Animations de marche
	walk_anims_[UP]		= &GET_ANIM("player_walk_top");
	walk_anims_[DOWN]	= &GET_ANIM("player_walk_bottom");
	walk_anims_[LEFT]	= &GET_ANIM("player_walk_left");
	walk_anims_[RIGHT]	= &GET_ANIM("player_walk_right");
	
	fall_anim_ = &GET_ANIM("player_fall");
	
	// Subrects d'immobilité
	subrects_not_moving_[UP]	= sf::IntRect(0,   0, 32,  48);
	subrects_not_moving_[DOWN]	= sf::IntRect(0,  48, 32,  96);
	subrects_not_moving_[LEFT]	= sf::IntRect(0,  96, 32, 144);
	subrects_not_moving_[RIGHT]	= sf::IntRect(0, 144, 32, 196);
	
	// attribution des touches de déplacement
	move_keys_[UP] = sf::Key::Up;
	move_keys_[DOWN] = sf::Key::Down;
	move_keys_[LEFT] = sf::Key::Left;
	move_keys_[RIGHT] = sf::Key::Right;
	
	// le joueur est de face par défaut
	current_dir_ = DOWN;
	was_moving_ = false;
	SetSubRect(subrects_not_moving_[DOWN]);
	
	SetCenter(0, subrects_not_moving_[DOWN].GetHeight());
	
	lives_ = 1;
	rupees_ = 42;
	locked_ = false;
	falling_ = false;
	
	panel_.SetLives(lives_);
	panel_.SetRupees(rupees_);
}


void Player::OnEvent(sf::Key::Code key)
{
	if (key == sf::Key::Space)
	{
		std::cerr << " -- DEBUG -- \n";
		std::cout << "GetPosition: " << GetPosition().x << ", " << GetPosition().y << ";\n"; 
	}
	
	else if (key == sf::Key::S)
	{
		ControlPanel::GetInstance().AddLifeSlot();
		std::cerr << "Added life slot to panel.\n";
	}
	else if (key == sf::Key::A)
	{
		lives_ += 1;
		ControlPanel::GetInstance().SetLives(lives_);
		std::cerr << "Added life to panel.\n";
	}
	else if (key == sf::Key::D)
	{
		lives_ -= 1;
		ControlPanel::GetInstance().SetLives(lives_);
		std::cerr << "Took away life from panel.\n";
		if (lives_ == 0)
		{
			puts("you're dead, bastard!");
			abort();
			dead_ = true;
		}
	}
	else if (key == sf::Key::T)
	{
		int i = (int)GetPosition().x + GetFloorWidth() / 2;
		int j = (int) GetPosition().y - GetFloorHeight() / 2;
	
		i /= Tile::SIZE;
		j /= Tile::SIZE;
		
		int tile = zone_->GetTileAt(i, j);
		Tile::Effect effect = Tileset::GetInstance().GetEffect(tile);
		std::cerr << "A la tile [" << tile << "]" << i << ", " << j << ", on a:\t";
		if (effect == Tile::HOLE)
		{
			puts("Trou >_>");
		}
		else if (effect == Tile::WATER)
		{
			puts("Eau");
		}
		else if (effect == Tile::DEFAULT)
		{
			puts("Default");
		}
		else
		{
			puts("Bloc");
		}
	}
}


void Player::Update(float frametime)
{
	if (locked_)
		return;
	
// <HACK>
	static ItemData i_d;
	static sf::Vector2f offset(0, 0);
	i_d.img_world_ = new char[8]; // FIXME: fuite mémoire
	i_d.name_ = "FAKE";
	strcpy(i_d.img_world_, "objects");
	static Item other(offset, i_d);
// </HACK>
	static Item* ptr = NULL;

	static Game& game = Game::GetInstance();
	
	static int tile;
	
	static float fall_timer;
	
	if (! falling_)
	{
		bool moved = false;
		int dx, dy;
		sf::FloatRect rect;
	
		// Chûte-t'on ?
		int i = (int) GetPosition().x + GetFloorWidth() / 2;
		int j = (int) GetPosition().y - GetFloorHeight() / 2;
		
		i /= Tile::SIZE;
		j /= Tile::SIZE;
		
		tile = zone_->GetTileAt(i, j);
		Tile::Effect effect = Tileset::GetInstance().GetEffect(tile);

		if (effect == Tile::HOLE)
		{
			puts("Trou >_>");
			falling_ = true;
			Animated::Change(fall_anim_, *this);
			fall_timer = FALL_DELAY;
			return;
		}
	
		//
	
		Direction new_dir;
		for (int dir = 0; dir < COUNT_DIRECTION; ++dir)
		{
			if (input_.IsKeyDown(move_keys_[dir]))
			{
				moved = true;
				new_dir = (Direction) dir;
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
				if (rect.Left < 0)
				{
					game.ChangeZone(Game::LEFT);
					break;
				}
				else if (rect.Top < 0) 
				{
					game.ChangeZone(Game::UP);
					break;
				}
				else if (rect.Right > Zone::WIDTH * Tile::SIZE)
				{
					game.ChangeZone(Game::RIGHT);
					break;
				}
				else if (rect.Bottom > Zone::HEIGHT * Tile::SIZE)
				{
					game.ChangeZone(Game::DOWN);
					break;
				}
				
				Zone::TileContent tc = zone_->CanMove(this, rect, ptr);
				if (tc == Zone::EMPTY )
				{
					SetPosition(pos);
				}
				else if (tc == Zone::DYNAMIC_NO)
				{
					puts("Item interactif overlappant!");
					std::cout << "\t" << ptr->name_ << "\n";
					if (ptr->name_ == "Rupee")
					{
						rupees_ += ptr->Take();
						panel_.SetRupees(rupees_);
					}
					else if (ptr->name_ == "Heart")
					{
						lives_ += ptr->Take();
						panel_.SetLives(lives_);
					}
				}
			}
		}
		// si on a bougé
		if (moved)
		{
			if (new_dir != current_dir_)
			{
				current_dir_ = new_dir;
				Animated::Change(walk_anims_[new_dir], *this);
			}
			Animated::Update(frametime, *this);
			was_moving_ = true;
		}
		else if (was_moving_)
		{
			was_moving_ = false;
			SetSubRect(subrects_not_moving_[current_dir_]);
		}
	}
	else
	{	// En cours de chûte...

		fall_timer -= frametime;
		Animated::Update(frametime, *this);
		if (fall_timer <= 0)
		{
			std::cerr << "EffectArg:" << zone_->GetEffectArg(tile) << "\n";
			if (zone_->GetEffectArg(tile) == 42)
			{
				game.Teleport("cave1");
			}
			else
			{
				lives_ -= 1;
				panel_.SetLives(lives_);
				SetPosition(Tile::SIZE, 8 * Tile::SIZE);
			}
			Animated::Change(walk_anims_[UP], *this);
			falling_ = false;
		}
	}
}


void Player::Lock()
{
	locked_ = true;
}


void Player::Unlock()
{
	locked_ = false;
}

