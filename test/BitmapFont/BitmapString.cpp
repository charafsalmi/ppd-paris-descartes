#include <cstring>

#include "BitmapString.hpp"


BitmapString::BitmapString()
{
	font_ = NULL;
}


void BitmapString::SetText(const char* text)
{
	text_.clear();
	const char* p = text;
	while (*p != '\0')
	{
		AppendChar(*p);
		++p;
	}
}


void BitmapString::SetText(const std::string& text)
{
	SetText(text.c_str());
}


void BitmapString::AppendChar(char character)
{
	sf::Sprite sprite;
	sf::IntRect subrect;
	font_->GetCharRect(character, subrect);
	
	sprite.SetImage(font_->GetImage());
	sprite.SetSubRect(subrect);

	size_t length = text_.size();
	if (length > 0)
	{
		sprite.SetX(length * font_->GetCharWidth());
	}
	text_.push_back(sprite);
}


void BitmapString::InsertChar(char character, int position)
{

}


void BitmapString::RemoveChar(int position)
{

}


void BitmapString::SetFont(const BitmapFont& font)
{
	font_ = &font;
}


void BitmapString::Clear()
{
	text_.clear();
}


int BitmapString::Length() const
{
	return text_.size();
}


void BitmapString::Render(sf::RenderTarget& target) const
{
	for (SpriteVec::const_iterator it = text_.begin(); it != text_.end(); ++it)
	{
		target.Draw(*it);
	}
}

