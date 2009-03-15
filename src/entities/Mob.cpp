#include "Mob.hpp"
#include "Hit.hpp"
#include "Player.hpp"
#include "../core/Zone.hpp"
#include "../core/Game.hpp"

#define FIRE_RATE     (1 / 1.f)   // (1 / tirs par seconde)


Mob::Mob(const sf::Vector2f& pos, const sf::Image& image, int hp, int speed) :
	Unit(pos, image)
{
	hp_ = hp;
	speed_ = speed;
	current_dir_ = RIGHT;
	last_hit_ = 0;
	// warning: animations non initialisées (rôle rempli par EntityFactory)
}


void Mob::AutoUpdate(float frametime)
{
	//Unit::AutoUpdate(frametime);
	sf::Vector2f pos = GetPosition();
	sf::FloatRect rect;
	rect.Left = pos.x + speed_ * frametime;
	rect.Bottom = pos.y;
	rect.Right = rect.Left + GetFloorWidth();
	rect.Top = rect.Bottom - GetFloorHeight();
	if (zone_->CanMove(rect))
	{
		Animated::Update(frametime, *this);
		SetX(rect.Left);
	}
	else
	{
		speed_ *= -1;
		if (current_dir_ == LEFT)
		{
			current_dir_ = RIGHT;
		}
		else
		{
			current_dir_ = LEFT;
		}
		Animated::Change(walk_anims_[current_dir_], *this);
		return;
	}

	static const Game& game = Game::GetInstance();
	Player* player = game.GetPlayer();
	pos = player->GetPosition();
	if ((pos.y < rect.Bottom && pos.y > rect.Top) || (pos.x > rect.Left && pos.x < rect.Right))
	{
		ThrowHit();
	}
}


void Mob::OnCollide(Entity& entity)
{
	if (dynamic_cast<Hit*>(&entity) == NULL)
	{
		speed_ *= -1;
		if (current_dir_ == LEFT)
		{
			current_dir_ = RIGHT;
		}
		else
		{
			current_dir_ = LEFT;
		}
		Animated::Change(walk_anims_[current_dir_], *this);
	}
}


void Mob::SetDirection(Direction dir)
{
	current_dir_ = dir;
	Animated::Change(walk_anims_[current_dir_], *this);
}


void Mob::ThrowHit()
{
	float now = Game::GetInstance().GetElapsedTime();
	if ((now - last_hit_) > FIRE_RATE)
	{
		sf::Vector2f pos(GetPosition().x + GetSize().x / 2, GetPosition().y - GetSize().y / 2);
		zone_->AddEntity(new Hit(pos, 2, current_dir_, GetID()));
		last_hit_ = now;
	}
}
