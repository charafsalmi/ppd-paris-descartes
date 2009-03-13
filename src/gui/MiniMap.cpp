#include "MiniMap.hpp"

#define DISCOVERED_COLOR   sf::Color(0xC0, 0xC0, 0xFF)
#define UNDISCOVERED_COLOR sf::Color(0x40, 0x40, 0x40)
#define OUTLINE 3

#define WIDTH_PX   200
#define HEIGHT_PX  200


MiniMap::MiniMap(const ZoneContainer& zones)
{
	width_ = zones.GetWidth();
	height_ = zones.GetHeight();

	int square_size = std::min(WIDTH_PX / width_, HEIGHT_PX / height_);

	preview_ = new sf::Shape* [height_];
	for (int y = 0; y < height_; ++y)
	{
		preview_[y] = new sf::Shape [width_];
		for (int x = 0; x < width_; ++x)
		{
			preview_[y][x] = sf::Shape::Rectangle(x * square_size, y * square_size,
				(x * square_size) + square_size, (y * square_size) + square_size,
				sf::Color::White, OUTLINE, sf::Color::Black);
			preview_[y][x].SetColor(UNDISCOVERED_COLOR);
		}
	}
	player_pos_ = NULL;
	square_size_ = square_size;
}


MiniMap::~MiniMap()
{
	for (int y = 0; y < height_; ++y)
	{
		delete [] preview_[y];
	}
	delete [] preview_;
}


int MiniMap::GetWidth() const
{
	return WIDTH_PX;
}


int MiniMap::GetHeight() const
{
	return HEIGHT_PX;
}


void MiniMap::SetPlayerPosition(const sf::Vector2i& coords)
{
	int y = coords.y * square_size_;
	int x = coords.x * square_size_;
	if (player_pos_ != NULL)
	{
		player_pos_->SetColor(DISCOVERED_COLOR);
	}
	player_pos_ = &preview_[coords.y][coords.x];
	player_pos_->SetColor(sf::Color::Red);
}


void MiniMap::Update(float frametime)
{
	// TODO
}

void MiniMap::Render(sf::RenderTarget& target) const
{
	for (int y = 0; y < height_; ++y)
	{
		for (int x = 0; x < width_; ++x)
		{
			target.Draw(preview_[y][x]);
		}
	}
}
