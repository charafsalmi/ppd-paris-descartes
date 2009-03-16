#include <SFML/System.hpp>

#include "Mob.hpp"
#include "Hit.hpp"
#include "Player.hpp"
#include "../core/Zone.hpp"
#include "../core/SoundSystem.hpp"
#include "../core/Game.hpp"

#define FIRE_RATE     (1 / 1.f)   // (1 / tirs par seconde)
#define DROP_LUCK     33          // percent


Mob::Mob(const sf::Vector2f& pos, const sf::Image& image, int hp, int speed) :
	Unit(pos, image)
{
	SetHP(hp);
	speed_ = speed;
	current_dir_ = RIGHT;
	last_hit_ = 0;
	// warning: animations non initialisées (rôle rempli par EntityFactory)
}


void Mob::AutoUpdate(float frametime)
{
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
	if (IsDying())
	{
		return;
	}
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


void Mob::Kill()
{
	// roll the dice!
	if (sf::Randomizer::Random(0, 100) < DROP_LUCK)
	{
		// TODO: définir quels sont les items dropables par un mob
		// TODO 2: pondérer certains items ayant plus de chances d'être droppés que d'autres
		// TODO 3: définir ça dans Item, puis définir la liste des items droppables pour
		// chaque mob dans son profil XML (?)
		// -> post-PPD (?)
		int item_id = sf::Randomizer::Random(1, 2); // coeur de vie ou argent

		zone_->AddItem(item_id, GetPosition().x, GetPosition().y);
	}
	Entity::Kill();
}


void Mob::TakeDamage(int damage)
{
	SoundSystem::GetInstance().PlaySound("mob-damage");
	Unit::TakeDamage(damage);
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
