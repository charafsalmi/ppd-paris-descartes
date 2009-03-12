#include "TextBox.hpp"
#include "../misc/MediaManager.hpp"

#define PADDING 3 // marge en pixels autour du texte
#define DEFAULT_WIDTH   100
#define CURSOR_WIDTH    2
#define CURSOR_COLOR    sf::Color::Blue

using namespace gui;


TextBox::TextBox(ControlID id, const ControlPos& pos, int width) :
	Control(id, pos),
	text_(GET_BITMAP_FONT("mono12-black"))
{
	int height = text_.GetFont().GetCharHeight() + PADDING * 2;
	if (width == UNDEFINED)
	{
		width = DEFAULT_WIDTH;
	}
	rect_.Left = pos.x;
	rect_.Top = pos.y;
	rect_.Right = pos.x + width;
	rect_.Bottom = pos.y + height;

	// postionnment relatif des éléments internes
	background_ = sf::Shape::Rectangle(0, 0, width, height,
		sf::Color::White, 3, sf::Color::Black);
	text_.SetPosition(PADDING, PADDING);
	cursor_ = sf::Shape::Line(PADDING, PADDING, PADDING, height - PADDING,
		CURSOR_WIDTH, CURSOR_COLOR);
	// test
	text_.SetText("pouet");
	SetCursor(0);
}


void TextBox::OnTextEntered(sf::Uint32 unicode)
{
	if (unicode >= BitmapFont::FIRST_CHAR && unicode <= BitmapFont::LAST_CHAR)
	{
		text_.InsertChar((char) unicode, cursor_pos_);
		SetCursor(cursor_pos_ + 1);
	}
}


void TextBox::OnKeyPressed(sf::Key::Code key)
{
	switch (key)
	{
		case sf::Key::Back:
			text_.RemoveChar(cursor_pos_);
			SetCursor(cursor_pos_ - 1);
			break;
		case sf::Key::Left:
			SetCursor(cursor_pos_ - 1);
			break;
		case sf::Key::Right:
			SetCursor(cursor_pos_ + 1);
			break;
		default:
			break;
	}
}


void TextBox::Render(sf::RenderTarget& app) const
{
	app.Draw(background_);
	app.Draw(text_);
	app.Draw(cursor_);
}


void TextBox::SetCursor(int position)
{
	if (position >= 0 && position < text_.Length() + 1)
	{
		cursor_pos_ = position;
		cursor_.SetX(position * text_.GetFont().GetCharWidth());
	}
}
