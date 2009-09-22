#include <SFML/System.hpp>

#include "Mob.hpp"
#include "Player.hpp"
#include "Equipment.hpp"
#include "../core/Game.hpp"
#include "../core/SoundSystem.hpp"

#define DROP_LUCK          33         // chance de dropper un item (en pourcent)
#define WALK_MIN_DURATION  2.f
#define WALK_MAX_DURATION  4.f


Mob::Mob(const sf::Vector2f& pos, const sf::Image& image, int hp, int speed) :
	Unit(pos, image, hp, speed)
{
	equipment_ = NULL;
}


Mob::~Mob()
{
	RemoveEquipment();
}


void Mob::AutoUpdate(float frametime)
{
	static const Game& game = Game::GetInstance();
	sf::Vector2f pos = GetPosition();
	int dx = 0, dy = 0;
	sf::FloatRect player;
	sf::FloatRect rect;
	game.GetPlayer()->GetGlobalRect(player);
	GetGlobalRect(rect);
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
	if (shot && equipment_ != NULL)
	{
		equipment_->Use<Hit>();
	}
}


void Mob::Kill()
{
	// roll the dice!
	if (sf::Randomizer::Random(0, 100) < DROP_LUCK)
	{
		// TODO: définir quels sont les items dropables par un mob plus proprement
		static const Item::Type DROPABLES[] = {
			Item::ITM_HEART,
			Item::ITM_RUBY,
		};

		int item = sf::Randomizer::Random(0, (sizeof (DROPABLES) / sizeof (Item::Type)) - 1);

		zone_->AddItem((Item::Type) item, GetPosition().x, GetPosition().y);
	}
	Entity::Kill();
}


void Mob::TakeDamage(int damage)
{
	SoundSystem::GetInstance().PlaySound("mob-damage");
	Unit::TakeDamage(damage);
}


void Mob::SetEquipment(Equipment* equipment)
{
	RemoveEquipment();
	if (equipment != NULL)
	{
		equipment->SetOwner(this);
	}
	equipment_ = equipment;
}


void Mob::ChooseDirection()
{
	started_at_ = Game::GetInstance().GetElapsedTime();
	walk_duration_ = sf::Randomizer::Random(WALK_MIN_DURATION, WALK_MAX_DURATION);
	SetDirection((Direction) sf::Randomizer::Random(0, COUNT_DIRECTION - 1));
}


void Mob::RemoveEquipment()
{
	if (equipment_ != NULL)
	{
		delete equipment_;
	}
	equipment_ = NULL;
}

