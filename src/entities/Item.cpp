#include "Item.hpp"
#include "../misc/MediaManager.hpp"
#include "Player.hpp"


Item::Item(const sf::Vector2f& pos, const char* name)
{
	SetPosition(pos);
	name_ = name;
	dead_ = false;
	SetImage(GET_IMG("items"));
}


void Item::GetRect(sf::FloatRect& rect) const
{
	rect.Left = GetPosition().x;
	rect.Top = GetPosition().y;
	rect.Right = rect.Left + GetSize().x;
	rect.Bottom = rect.Top + GetSize().y;
}


const std::string& Item::GetName() const
{
	return name_;
}


void Item::Kill()
{
	dead_ = true;
}


Money::Money(const sf::Vector2f& pos):
	Item(pos, "Rupee")
{
	SetSubRect(sf::IntRect(20, 0, 20 + 14, 0 + 28));
}


void Money::OnCollide(Player& player)
{
	player.AddMoney();
	Kill();
}


Heart::Heart(const sf::Vector2f& pos):
	Item(pos, "Coeur")
{
	SetSubRect(sf::IntRect(2, 4, 2 + 14, 4 + 14));
}


void Heart::OnCollide(Player& player)
{
	player.AddLife();
	Kill();
}

