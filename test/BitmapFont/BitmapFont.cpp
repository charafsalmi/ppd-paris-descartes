#include "BitmapFont.hpp"

#define FIRST_CHAR 32


BitmapFont::BitmapFont(int width, int height, int width_count, int height_count)
{
	width_ = width;
	height_ = height;
	width_count_ = width_count;
	height_count_ = height_count;
	image_ = NULL;
}


void BitmapFont::SetImage(const sf::Image& image)
{
	image_ = &image;
}


const sf::Image& BitmapFont::GetImage() const
{
	return *image_;
}


void BitmapFont::GetCharRect(char character, sf::IntRect& subrect) const
{

	character -= FIRST_CHAR;
	if (character < 0)
	{
		character = 0;
		puts("warning: bad character (< 20)");
	}
	subrect.Left = (character % width_count_) * width_;
	subrect.Right = subrect.Left + width_;
	subrect.Top = (character / width_count_) * height_;
	subrect.Bottom = subrect.Top + height_;
}


int BitmapFont::GetCharWidth() const
{
	return width_;
}

