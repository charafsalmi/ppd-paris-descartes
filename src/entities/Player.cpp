#include <cstring>
#include <typeinfo>

#include "Player.hpp"
#include "PlayerHit.hpp"
#include "Mob.hpp"
#include "Equipment.hpp"

#include "../core/Game.hpp"
#include "../core/SoundSystem.hpp"
#include "../misc/MediaManager.hpp"
#include "../core/Tileset.hpp"
#include "../core/Zone.hpp"
#include "../misc/Log.hpp"

#define WALK_SPEED  130
#define SWIM_SPEED  70
#define DEFAULT_HP  10
#define FIRE_RATE   (1 / 2.f)   // (1 / tirs par seconde)

#define ACCEPTED_TILES (Tile::DEFAULT | Tile::WATER | Tile::TELEPORT | Tile::HOLE)


Player::Player(const sf::Vector2f& pos) :
	Unit(pos, GET_IMG("player"), DEFAULT_HP, WALK_SPEED),
	panel_(ControlPanel::GetInstance())
{
	// valeurs magiques... surface de contact au sol
	SetFloor(28, 20);

	// Animations de marche
	walk_anims_[UP]		= &GET_ANIM("player_walk_up");
	walk_anims_[DOWN]	= &GET_ANIM("player_walk_down");
	walk_anims_[LEFT]	= &GET_ANIM("player_walk_left");
	walk_anims_[RIGHT]	= &GET_ANIM("player_walk_right");

	// Subrects d'immobilité
	subrects_not_moving_[UP]	= sf::IntRect(0,  0, 32,  48);
	subrects_not_moving_[DOWN]	= sf::IntRect(32, 0, 64,  48);
	subrects_not_moving_[LEFT]	= sf::IntRect(64, 0, 96,  48);
	subrects_not_moving_[RIGHT]	= sf::IntRect(96, 0, 128, 48);

	SetCenter(0, walk_anims_[DOWN]->GetFrame(0).GetHeight());

	// animation de chûte
	fall_anim_ = &GET_ANIM("player_fall");

	// attribution des touches de déplacement
	move_keys_[UP] = input::MOVE_UP;
	move_keys_[DOWN] = input::MOVE_DOWN;
	move_keys_[LEFT] = input::MOVE_LEFT;
	move_keys_[RIGHT] = input::MOVE_RIGHT;

	// le joueur est de face par défaut
	SetDirection(DOWN);
	was_moving_ = false;
	SetSubRect(subrects_not_moving_[DOWN]);

	max_lives_ = DEFAULT_HP;
	money_ = 0;
	frags_ = 0;

	panel_.SetHP(max_lives_);
	panel_.SetGold(money_);
	panel_.SetFragCount(frags_);

	panel_.SetItem1(NULL);
	panel_.SetItem2(NULL);
	panel_.SetItem3(NULL);

	locked_ = false;
	last_hit_ = 0;
	can_use_item_ = true;
	// on suppose que toutes les directions d'une animation ont la même durée (FIX ME?)
	use_bow_duration_ = GET_ANIM("player_bow_up").GetDuration();
	use_sword_duration_ = GET_ANIM("player_sword_up").GetDuration();
	falling_duration_ = GET_ANIM("player_fall").GetDuration();

	strategy_callback_ = &Player::WalkUpdate;
	equipment_ = NULL;
	last_good_position_ = pos;
}


void Player::OnEvent(input::Action action)
{
	int item = Item::ITM_NONE;
	switch (action)
	{
		case input::USE_ITEM_1:
			item = inventory_.GetItem1Type();
			break;
		case input::USE_ITEM_2:
			item = inventory_.GetItem2Type();
			break;
		case input::USE_ITEM_3:
			item = inventory_.GetItem3Type();
			break;
		default:
			return;
	}
	if (item != Item::ITM_NONE)
	{
		UseItem((Item::Type) item);
	}
}


void Player::AutoUpdate(float frametime)
{
	if (!locked_)
	{
		(this->*strategy_callback_)(frametime);
	}
}


void Player::HandleInput(float frametime)
{
	static Game& game = Game::GetInstance();
	static const InputController& input_ = InputController::GetInstance();

	// déplacement selon les touches appuyées
	bool moved = false;
	int dx, dy;
	sf::FloatRect rect;
	Direction new_dir;
	for (int dir = 0; dir < COUNT_DIRECTION; ++dir)
	{
		if (input_.HasInput(move_keys_[dir]))
		{
			moved = true;
			new_dir = (Direction) dir;
			dx = dy = 0;
			switch (dir)
			{
				case UP:
					dy = -GetSpeed();
					break;
				case DOWN:
					dy = GetSpeed();
					break;
				case LEFT:
					dx = -GetSpeed();
					break;
				case RIGHT:
					dx = GetSpeed();
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

			if (zone_->CanMove(rect, ACCEPTED_TILES))
			{
				last_good_position_ = GetPosition();
				SetPosition(pos);
			}
		}
	}
	// si on a bougé
	if (moved)
	{
		if (new_dir != GetDirection())
		{
			SetDirection(new_dir);
		}
		Animated::Update(frametime, *this);
		was_moving_ = true;
	}
	else if (was_moving_)
	{
		was_moving_ = false;
		SetSubRect(subrects_not_moving_[GetDirection()]);
	}
}


void Player::WalkUpdate(float frametime)
{
	static int tile;
	int i, j;
	GetTilePosition(i, j);

	// si trou, on chûte
	tile = zone_->GetTileAt(i, j);
	Tile::Effect effect = Tileset::GetInstance().GetEffect(tile);
	if (effect == Tile::HOLE)
	{
		Animated::Change(fall_anim_, *this);
		started_action_ = Game::GetInstance().GetElapsedTime();
		strategy_callback_ = &Player::FallingUpdate;
		return;
	}

	// si téléporteur, on se téléporte à la destination
	if (effect == Tile::TELEPORT)
	{
		Zone::Teleporter tp;
		if (zone_->GetTeleport(i, j, tp))
		{
		    Output << PLAYER_S << "Joueur active un teleporteur" << lEnd;
			Game::GetInstance().Teleport(tp);
			return;
		}
	}

	// si eau, on passe en mode nage
	if (effect == Tile::WATER)
	{
		walk_anims_[UP]		= &GET_ANIM("player_swim_up");
		walk_anims_[DOWN]	= &GET_ANIM("player_swim_down");
		walk_anims_[LEFT]	= &GET_ANIM("player_swim_left");
		walk_anims_[RIGHT]	= &GET_ANIM("player_swim_right");

		subrects_not_moving_[UP]	= walk_anims_[UP]->GetFrame(0);
		subrects_not_moving_[DOWN]	= walk_anims_[DOWN]->GetFrame(0);
		subrects_not_moving_[LEFT]	= walk_anims_[LEFT]->GetFrame(0);
		subrects_not_moving_[RIGHT]	= walk_anims_[RIGHT]->GetFrame(0);

		Animated::Change(walk_anims_[GetDirection()], *this);
		SetSpeed(SWIM_SPEED);
		can_use_item_ = false;
		strategy_callback_ = &Player::SwimmingUpdate;

	}
	HandleInput(frametime);
}


void Player::SwimmingUpdate(float frametime)
{
	int i, j;
	GetTilePosition(i, j);
	Tile::Effect effect = Tileset::GetInstance().GetEffect(zone_->GetTileAt(i, j));
	if (effect == Tile::DEFAULT || effect == Tile::HOLE)
	{
		walk_anims_[UP]		= &GET_ANIM("player_walk_up");
		walk_anims_[DOWN]	= &GET_ANIM("player_walk_down");
		walk_anims_[LEFT]	= &GET_ANIM("player_walk_left");
		walk_anims_[RIGHT]	= &GET_ANIM("player_walk_right");

		subrects_not_moving_[UP]	= sf::IntRect(0,  0, 32,  48);
		subrects_not_moving_[DOWN]	= sf::IntRect(32, 0, 64,  48);
		subrects_not_moving_[LEFT]	= sf::IntRect(64, 0, 96,  48);
		subrects_not_moving_[RIGHT]	= sf::IntRect(96, 0, 128, 48);

		Animated::Change(walk_anims_[GetDirection()], *this);
		SetSpeed(WALK_SPEED);
		can_use_item_ = true;
		strategy_callback_ = &Player::WalkUpdate;
	}
	HandleInput(frametime);
}


void Player::FallingUpdate(float frametime)
{
	Animated::Update(frametime, *this);
	float now = Game::GetInstance().GetElapsedTime();
	if ((now - started_action_) > falling_duration_)
	{
		// chûte terminée, soit c'est un téléporteur, soit c'est un trou normal
		Zone::Teleporter tp;
		int i, j;
		GetTilePosition(i, j);
		if (zone_->GetTeleport(i, j, tp))
		{
			Game::GetInstance().Teleport(tp);
		}
		else
		{
			TakeDamage(1);
			SetPosition(last_good_position_);
		}
		Animated::Change(walk_anims_[GetDirection()], *this);
		SetSubRect(subrects_not_moving_[GetDirection()]); // ?
		strategy_callback_ = &Player::WalkUpdate;
		can_use_item_ = true;
	}
}


void Player::UseBowUpdate(float frametime)
{
	float now = Game::GetInstance().GetElapsedTime();
	if ((now - started_action_) > use_bow_duration_)
	{
		//ThrowHit(Hit::ARROW);
		Animated::Change(walk_anims_[GetDirection()], *this);
		SetSubRect(subrects_not_moving_[GetDirection()]); // ?
		strategy_callback_ = &Player::WalkUpdate;
	}
	else
	{
		Animated::Update(frametime, *this);
	}
}


void Player::UseSwordUpdate(float frametime)
{
	float now = Game::GetInstance().GetElapsedTime();
	if ((now - started_action_) > use_sword_duration_)
	{
		Animated::Change(walk_anims_[GetDirection()], *this);
		SetSubRect(subrects_not_moving_[GetDirection()]); // ?
		strategy_callback_ = &Player::WalkUpdate;
	}
	else
	{
		Animated::Update(frametime, *this);
	}
}


void Player::OnCollide(Entity& entity, const sf::FloatRect& overlap)
{
	Unit::OnCollide(entity, overlap);
	if (typeid (entity) == typeid (Mob) && !entity.IsDying())
	{
		TakeDamage(1);
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


void Player::SetHP(int hp)
{
	Unit::SetHP(hp);
	panel_.SetHP(hp);
}


void Player::AddHP()
{
	int hp = GetHP();
	if (hp < max_lives_)
	{
		++hp;
		SetHP(hp);
		panel_.SetHP(hp);
	}
}


void Player::AddGold(int amount)
{
	money_ += amount;
	panel_.SetGold(money_);
}


int Player::GetMoney() const
{
	return money_;
}


void Player::AddFrag(int amount)
{
	frags_ += amount;
	panel_.SetFragCount(frags_);
}


int Player::GetFrags() const
{
	return frags_;
}


void Player::Kill()
{
	Entity::Kill();
	Game::GetInstance().EndGame();
}


void Player::TakeDamage(int damage)
{
	if (!IsDying())
	{
		Unit::TakeDamage(damage);
		panel_.SetHP(GetHP());
		SoundSystem::GetInstance().PlaySound("player-damage");
	}
}


bool Player::AddEquipment(Item::Type type)
{
	if (!inventory_.HasItem(type))
	{
		Equipment* equip = new Equipment(type);
		equip->SetOwner(this);
		inventory_.AddItem(equip);
		return true;
	}
	return false;
}


void Player::SetEquipment(Equipment* equipment)
{
	equipment->SetOwner(this);
	inventory_.AddItem(equipment);
}


void Player::ThrowHit(Hit::Type type)
{
	float now = Game::GetInstance().GetElapsedTime();
	if ((now - last_hit_) > FIRE_RATE)
	{
		sf::Vector2f pos(GetPosition().x + GetSize().x / 2, GetPosition().y - GetSize().y / 2);
		zone_->AddEntity(new PlayerHit(pos, 2, GetDirection(), GetID(), type));
		last_hit_ = now;
	}
}


void Player::UseItem(Item::Type type)
{
	if (!can_use_item_)
	{
		return;
	}
	switch (type)
	{
		case Item::ITM_SWORD:
			switch (GetDirection())
			{
				case UP:
					Animated::Change(&GET_ANIM("player_sword_up"), *this);
					break;
				case DOWN:
					Animated::Change(&GET_ANIM("player_sword_down"), *this);
					break;
				case LEFT:
					Animated::Change(&GET_ANIM("player_sword_left"), *this);
					break;
				case RIGHT:
					Animated::Change(&GET_ANIM("player_sword_right"), *this);
					break;
				default:
					break;
			}
			strategy_callback_ = &Player::UseSwordUpdate;
			started_action_ = Game::GetInstance().GetElapsedTime();
			ThrowHit(Hit::SWORD);
			break;
		case Item::ITM_BOW:
			switch (GetDirection())
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
			ThrowHit(Hit::ARROW);
			strategy_callback_ = &Player::UseBowUpdate;
			started_action_ = Game::GetInstance().GetElapsedTime();
			break;
		default:
			break;
	}
}


void Player::GetTilePosition(int& i, int& j)
{
	i = ((int) GetPosition().x + GetFloorWidth() / 2) / Tile::SIZE;;
	j = ((int) GetPosition().y - GetFloorHeight() / 2) / Tile::SIZE;
}

