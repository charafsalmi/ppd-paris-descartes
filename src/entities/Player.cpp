#include <iostream>
#include <cstring>
#include <typeinfo>

#include "Player.hpp"
#include "PlayerHit.hpp"
#include "Mob.hpp"
#include "../core/Game.hpp"
#include "../misc/MediaManager.hpp"
#include "../core/Tileset.hpp"
#include "../core/Zone.hpp"

#define SPEED         120
#define FALL_DELAY    1
#define DEFAULT_LIVES 10
#define FIRE_RATE     (1 / 2.f)   // (1 / tirs par seconde)


Player::Player(const sf::Vector2f& pos, const sf::Input& input) :
	Unit(pos, GET_IMG("player")),
	input_(input),
	panel_(ControlPanel::GetInstance())
{
	// valeurs magiques... surface de contact au sol
	SetFloor(28, 20);

	// Animations de marche
	walk_anims_[UP]		= &GET_ANIM("player_walk_up");
	walk_anims_[DOWN]	= &GET_ANIM("player_walk_down");
	walk_anims_[LEFT]	= &GET_ANIM("player_walk_left");
	walk_anims_[RIGHT]	= &GET_ANIM("player_walk_right");
	Animated::Change(walk_anims_[DOWN], *this);
	SetCenter(0, walk_anims_[DOWN]->GetFrame(0).GetHeight());

	fall_anim_ = &GET_ANIM("player_fall");

	// Subrects d'immobilité
	subrects_not_moving_[UP]	= sf::IntRect(0,  0, 32,  48);
	subrects_not_moving_[DOWN]	= sf::IntRect(32, 0, 64,  48);
	subrects_not_moving_[LEFT]	= sf::IntRect(64, 0, 96,  48);
	subrects_not_moving_[RIGHT]	= sf::IntRect(96, 0, 128, 48);

	// attribution des touches de déplacement
	move_keys_[UP] = sf::Key::Up;
	move_keys_[DOWN] = sf::Key::Down;
	move_keys_[LEFT] = sf::Key::Left;
	move_keys_[RIGHT] = sf::Key::Right;

	// le joueur est de face par défaut
	current_dir_ = DOWN;
	was_moving_ = false;
	SetSubRect(subrects_not_moving_[DOWN]);

	hp_ = DEFAULT_LIVES;
	max_lives_ = DEFAULT_LIVES;
	money_ = 42;
	locked_ = false;
	falling_ = false;

	panel_.SetHP(hp_);
	panel_.SetMoney(money_);

	last_hit_ = 0;

	// on suppose que tirer une flèche prend autant de temps, quelque que soit la direction
	use_bow_duration_ = GET_ANIM("player_bow_up").GetDuration();
	falling_duration_ = GET_ANIM("player_fall").GetDuration();

	strategy_callback_ = &Player::WalkUpdate;
}


void Player::OnEvent(sf::Key::Code key)
{
	// <DEBUG HACK>
	int obj;
	switch (key)
	{
		case sf::Key::A:
			obj = panel_.GetInventory()->GetItem1ID();
			if (obj == 0)
				break;
			std::cout << "[Player]le joueur utilise l'objet " << obj << std::endl;
			if (obj == 11)
				ThrowHit();
			break;
		case sf::Key::Z:
			obj = panel_.GetInventory()->GetItem2ID();
			if (obj == 0)
				break;
			std::cout << "[Player]le joueur utilise l'objet " << obj << std::endl;
			if (obj == 11)
				ThrowHit();
			break;
		case sf::Key::E:
            obj = panel_.GetInventory()->GetItem3ID();
            if (obj == 0)
				break;
			std::cout << "[Player]le joueur utilise l'objet " << obj << std::endl;
			if (obj == 11)
				ThrowHit();
			break;
		case sf::Key::Space:
			switch (current_dir_)
			{
			case UP:
				Animated::Change(&GET_ANIM("player_bow_up"), *this);
				break;
			case DOWN:
				Animated::Change(&GET_ANIM("player_bow_down"), *this);
				break;
			case LEFT:
				Animated::Change(&GET_ANIM("player_bow_left"), *this);
				break;
			case RIGHT:
				Animated::Change(&GET_ANIM("player_bow_right"), *this);
				break;
			default:
				break;
			}
			strategy_callback_ = &Player::UseBowUpdate;
			started_action_ = Game::GetInstance().GetElapsedTime();
			break;
		// position
		case sf::Key::P:
			std::cout << " [Player] position: " << GetPosition().x << ", " << GetPosition().y << ";\n";
			break;
		// + 1 slot
		case sf::Key::S:
			++max_lives_;
			ControlPanel::GetInstance().AddLifeSlot();
			std::cout << " [Player] added life slot to panel.\n";
			break;
		// afficher la tile sous nos pieds
		case sf::Key::T:
		{
			int i = (int)GetPosition().x + GetFloorWidth() / 2;
			int j = (int) GetPosition().y - GetFloorHeight() / 2;

			i /= Tile::SIZE;
			j /= Tile::SIZE;

			int tile = zone_->GetTileAt(i, j);
			Tile::Effect effect = Tileset::GetInstance().GetEffect(tile);
			std::cout << " [Player] à la tile id: " << tile
				<< ", pos: (" << i << ", " << j << "), label: ";
			if (effect == Tile::HOLE)
			{
				puts("Trou");
			}
			else if (effect == Tile::WATER)
			{
				puts("Eau");
			}
			else if (effect == Tile::DEFAULT)
			{
				puts("Default");
			}
			else if (effect == Tile::TELEPORT)
			{
				puts("Téléport");
			}
		}
			break;
		// -1 hp (Ziane <3)
		case sf::Key::F2:
			TakeDamage(1);
			break;
		case sf::Key::F3:
			AddLife();
			break;
		default:
			break;

	}
	// </DEBUG HACK>
}


void Player::AutoUpdate(float frametime)
{
	//Unit::AutoUpdate(frametime);
	(this->*strategy_callback_)(frametime);
}


void Player::WalkUpdate(float frametime)
{
	if (locked_)
		return;

	static Game& game = Game::GetInstance();
	static int tile;
	static float fall_timer;
	int i = (int) GetPosition().x + GetFloorWidth() / 2;
	int j = (int) GetPosition().y - GetFloorHeight() / 2;

	i /= Tile::SIZE;
	j /= Tile::SIZE;

	if (!falling_)
	{
		bool moved = false;
		int dx, dy;
		sf::FloatRect rect;

		// Chûte-t'on ?
		tile = zone_->GetTileAt(i, j);
		Tile::Effect effect = Tileset::GetInstance().GetEffect(tile);
		if (effect == Tile::HOLE)
		{
			puts(" [Player] falling...");
			falling_ = true;
			Animated::Change(fall_anim_, *this);
			fall_timer = FALL_DELAY;
			return;
		}

		if (effect == Tile::TELEPORT)
		{
			Zone::Teleporter tp;
			if (zone_->GetTeleport(i, j, tp))
			{
				puts(" [Player] a activé un téléporteur !");
				game.Teleport(tp);
				return;
			}
		}

		// déplacement
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
					game.ChangeZone(ZoneContainer::LEFT);
					break;
				}
				else if (rect.Top < 0)
				{
					game.ChangeZone(ZoneContainer::UP);
					break;
				}
				else if (rect.Right > Zone::WIDTH * Tile::SIZE)
				{
					game.ChangeZone(ZoneContainer::RIGHT);
					break;
				}
				else if (rect.Bottom > Zone::HEIGHT * Tile::SIZE)
				{
					game.ChangeZone(ZoneContainer::DOWN);
					break;
				}

				if (zone_->CanMove(rect))
				{
					SetPosition(pos);
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
	{
		// En cours de chûte...
		fall_timer -= frametime;
		Animated::Update(frametime, *this);
		if (fall_timer <= 0)
		{
			Zone::Teleporter tp;
			if (zone_->GetTeleport(i, j, tp))
			{
				game.Teleport(tp);
			}
			else
			{
				--hp_;
				panel_.SetHP(hp_);
				SetPosition(Tile::SIZE, 8 * Tile::SIZE);
			}
			Animated::Change(walk_anims_[current_dir_], *this);
			falling_ = false;
		}
	}
}


void Player::FallingUpdate(float frametime)
{
	// TODO: virer le code de chûte de WalkUpdate ici.
}


void Player::UseBowUpdate(float frametime)
{
	float now = Game::GetInstance().GetElapsedTime();
	if ((now - started_action_) > use_bow_duration_)
	{
		ThrowHit();
		Animated::Change(walk_anims_[current_dir_], *this);
		SetSubRect(subrects_not_moving_[current_dir_]);
		strategy_callback_ = &Player::WalkUpdate;
	}
	else
	{
		Animated::Update(frametime, *this);
	}
}


void Player::OnCollide(Entity& entity)
{
	if (typeid (entity) == typeid (Mob))
	{
		TakeDamage(1);
		panel_.SetHP(hp_);
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


void Player::AddLife()
{
	if (hp_ < max_lives_)
	{
		++hp_;
		panel_.SetHP(hp_);
	}
}


void Player::AddMoney()
{
	++money_;
	panel_.SetMoney(money_);
}


void Player::Kill()
{
	Entity::Kill();
	Game::GetInstance().EndGame();
}


void Player::TakeDamage(int damage)
{
	Unit::TakeDamage(damage);
	panel_.SetHP(hp_);
}


void Player::ThrowHit()
{
	float now = Game::GetInstance().GetElapsedTime();
	if ((now - last_hit_) > FIRE_RATE)
	{
		sf::Vector2f pos(GetPosition().x + GetSize().x / 2, GetPosition().y - GetSize().y / 2);
		zone_->AddEntity(new PlayerHit(pos, 2, current_dir_, GetID()));
		last_hit_ = now;
	}
}
