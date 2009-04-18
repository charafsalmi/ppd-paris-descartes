#include <SFML/System.hpp>

#include "Mob.hpp"
#include "Hit.hpp"
#include "Player.hpp"
#include "../core/Zone.hpp"
#include "../core/Game.hpp"
#include "../core/SoundSystem.hpp"

#define FIRE_RATE          (1.f / 0.8f) // tirs par seconde
#define DROP_LUCK          33         // chance de dropper un item (en pourcent)
#define WALK_MIN_DURATION  2.f
#define WALK_MAX_DURATION  4.f


Mob::Mob(const sf::Vector2f& pos, const sf::Image& image, int hp, int speed) :
	Unit(pos, image, hp, speed)
{
	last_hit_ = 0;
}


void Mob::AutoUpdate(float frametime)
{
	static const Game& game = Game::GetInstance();
	sf::Vector2f pos = GetPosition();
	int dx = 0, dy = 0;
	sf::FloatRect player;
	sf::FloatRect rect;
	game.GetPlayer()->GetCollideRect(player);
	GetCollideRect(rect);
	bool shot = false;
	switch (GetDirection())
	{
		case UP:
			if (player.Bottom < rect.Top
				&& ((player.Left < rect.Right && player.Left > rect.Left)
				|| (player.Right < rect.Right && player.Right > rect.Left)))
			{
				shot = true;
			}
			dy = -GetSpeed();
			break;
		case DOWN:
			if (player.Top > rect.Bottom
				&& ((player.Left < rect.Right && player.Left > rect.Left)
				|| (player.Right < rect.Right && player.Right > rect.Left)))
			{
				shot = true;
			}
			dy = GetSpeed();
			break;
		case LEFT:
			if (player.Right < rect.Left
				&& ((player.Top < rect.Bottom && player.Top > rect.Top)
				|| (player.Bottom < rect.Bottom && player.Bottom > rect.Top)))
			{
				shot = true;
			}
			dx = -GetSpeed();
			break;
		case RIGHT:
			if (player.Left > rect.Right
				&& ((player.Top < rect.Bottom && player.Top > rect.Top)
				|| (player.Bottom < rect.Bottom && player.Bottom > rect.Top)))
			{
				shot = true;
			}
			dx = GetSpeed();
			break;
		default:
			break;
	}

	pos.x += dx * frametime;
	pos.y += dy * frametime;

	rect.Left = pos.x;
	rect.Bottom = pos.y;
	rect.Right = rect.Left + GetFloorWidth();
	rect.Top = rect.Bottom - GetFloorHeight();
	if (!zone_->CanMove(rect) ||
		(game.GetElapsedTime() - started_at_) > walk_duration_)
	{
		// bloqué ou temps écoulé, on change de direciton
		ChooseDirection();
		return;
	}

	Animated::Update(frametime, *this);
	SetPosition(pos.x, pos.y);
	// doit-on tirer ?
	if (shot)
	{
		ThrowHit();
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


void Mob::ThrowHit()
{
	float now = Game::GetInstance().GetElapsedTime();
	if ((now - last_hit_) > FIRE_RATE)
	{
		sf::Vector2f pos(GetPosition().x + GetSize().x / 2, GetPosition().y - GetSize().y / 2);
		zone_->AddEntity(new Hit(pos, 1, GetDirection(), GetID(), Hit::ARROW));
		last_hit_ = now;
	}
}


void Mob::ChooseDirection()
{
	started_at_ = Game::GetInstance().GetElapsedTime();
	walk_duration_ = sf::Randomizer::Random(WALK_MIN_DURATION, WALK_MAX_DURATION);
	SetDirection((Direction) sf::Randomizer::Random(0, COUNT_DIRECTION - 1));
}
