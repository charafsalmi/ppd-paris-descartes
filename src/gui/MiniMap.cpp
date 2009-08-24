#include "MiniMap.hpp"

#define DISCOVERED_COLOR   sf::Color(0xC0, 0xC0, 0xFF, 128)
#define UNDISCOVERED_COLOR sf::Color(0x40, 0x40, 0x40, 128)
#define PLAYER_COLOR       sf::Color(0xFF, 0,    0,    128)
#define OUTLINE 3

#define WIDTH_PX   160
#define HEIGHT_PX  160

#define BLINK_DELAY 1.4f


MiniMap::MiniMap(const ZoneContainer& zones)
{
	width_ = zones.GetWidth();
	height_ = zones.GetHeight();

	int square_size = std::min(WIDTH_PX / width_, HEIGHT_PX / height_);

	preview_ = new sf::Shape* [height_];
	for (int i = 0; i < height_; ++i)
	{
		preview_[i] = new sf::Shape [width_];
		for (int j = 0; j < width_; ++j)
		{
			int x = j * (square_size + OUTLINE);
			int y = i * (square_size + OUTLINE);
			preview_[i][j] = sf::Shape::Rectangle(x, y,
				x + square_size, y + square_size,
				sf::Color::White, OUTLINE, sf::Color::Black);
			preview_[i][j].SetColor(UNDISCOVERED_COLOR);
		}
	}
	player_pos_ = NULL;
	square_size_ = square_size;
	timer_ = 0;
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
	if (player_pos_ != NULL)
	{
		player_pos_->SetColor(DISCOVERED_COLOR);
	}
	player_pos_ = &preview_[coords.y][coords.x];
	player_pos_->SetColor(PLAYER_COLOR);
}


void MiniMap::Update(float frametime)
{
	timer_ -= frametime;
	if (timer_ <= 0)
	{
		timer_ = BLINK_DELAY;
	}
	else
	{
		float rate = 127 + (127 * timer_ / BLINK_DELAY);
		player_pos_->SetColor(sf::Color(0xFF, 0, 0, rate));
	}
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
