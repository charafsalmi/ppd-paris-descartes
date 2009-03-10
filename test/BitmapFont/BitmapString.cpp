#include <cstring>

#include "BitmapString.hpp"


BitmapString::BitmapString(const BitmapFont& font)
{
	font_ = &font;
	char_width_ = font_->GetCharWidth();
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


const char* BitmapString::GetText() const
{
	return chars_.c_str();
}


void BitmapString::AppendChar(char character)
{
	sf::Sprite sprite;
	sprite.SetImage(font_->GetImage());
	sprite.SetSubRect(font_->GetCharRect(character));

	size_t length = text_.size();
	if (length > 0)
	{
		sprite.SetX(length * char_width_);
	}
	text_.push_back(sprite);
	chars_ += character;
}


void BitmapString::InsertChar(char character, int position)
{
	if (position < 0)
	{
		position = chars_.size() - position - 2;
	}
	chars_.insert(position, 1, character);
	sf::Sprite sprite;
	// TODO
	//text_.insert(text_.begin() + position)
}


void BitmapString::RemoveChar(int position)
{
	if (chars_.empty())
	{
		return;
	}
	if (position < 0)
	{
		position = chars_.size() - position - 2;
	}
	chars_.erase(position);
	text_.erase(text_.begin() + position);
	
	printf("erase: moving %d bitmaps\n", text_.size() - position);
	
	for (int i = position; i < text_.size(); ++i)
	{
		sf::Sprite& sprite = text_[i];
		sprite.SetPosition(i * char_width_, 0);
	}
}


char BitmapString::GetCharAt(int position) const
{
	if (position >= 0)
	{
		return chars_[position];
	}
	return chars_[chars_.size() - position - 2];
}


void BitmapString::SetFont(const BitmapFont& font)
{
	if (&font != font_)
	{
		font_ = &font;
		char_width_ = font.GetCharWidth();
		for (int i = 0; i < text_.size(); ++i)
		{
			sf::Sprite& sprite = text_[i];
			sprite.SetX(i * char_width_);
			sprite.SetSubRect(font_->GetCharRect(chars_[i]));
			sprite.SetImage(font_->GetImage());
		}
	}
}


void BitmapString::Clear()
{
	text_.clear();
	chars_.clear();
}


int BitmapString::Length() const
{
	return chars_.size();
}


void BitmapString::Render(sf::RenderTarget& target) const
{
	for (SpriteVec::const_iterator it = text_.begin(); it != text_.end(); ++it)
	{
		target.Draw(*it);
	}
}

