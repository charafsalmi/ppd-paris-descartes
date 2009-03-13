#include "TextBox.hpp"
#include "../misc/MediaManager.hpp"

#define PADDING        3 // marge en pixels autour du texte
#define OUTLINE        1 // bordure de la textbox
#define DEFAULT_WIDTH  100
#define CURSOR_WIDTH   2
#define CURSOR_COLOR   sf::Color::Blue


using namespace gui;
using namespace std;


TextBox::TextBox(ControlID id, const ControlPos& pos, int width) :
	Control(id, pos),
	BStext_(GET_BITMAP_FONT("mono12-black"))
{
	int height = BStext_.GetFont().GetCharHeight() + PADDING * 2;
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
		sf::Color::White, OUTLINE, sf::Color::Black);
	BStext_.SetPosition(PADDING, PADDING);
	cursor_ = sf::Shape::Line(PADDING, PADDING, PADDING, height - PADDING,
		CURSOR_WIDTH, CURSOR_COLOR);
	SetCursor(0);
}


void TextBox::SetText(const string & s)
{
	BStext_.SetText(s.c_str());
}


string TextBox::GetText() const
{
    string s = BStext_.GetText();
	return s;
}


void TextBox::OnTextEntered(sf::Uint32 unicode)
{
	if (unicode >= BitmapFont::FIRST_CHAR && unicode <= BitmapFont::LAST_CHAR)
	{
		BStext_.InsertChar((char) unicode, cursor_pos_);
		SetCursor(cursor_pos_ + 1);
	}
}


void TextBox::OnKeyPressed(sf::Key::Code key)
{
	switch (key)
	{
		case sf::Key::Back:
			if (cursor_pos_ > 0)
			{
				BStext_.RemoveChar(cursor_pos_);
				SetCursor(cursor_pos_ - 1);
			}
			break;
		case sf::Key::Delete:
			if (cursor_pos_ < BStext_.Length())
			{
				BStext_.RemoveChar(cursor_pos_ + 1);
			}
			break;
		case sf::Key::Left:
			SetCursor(cursor_pos_ - 1);
			break;
		case sf::Key::Right:
			SetCursor(cursor_pos_ + 1);
			break;
		case sf::Key::Home:
			SetCursor(0);
			break;
		case sf::Key::End:
			SetCursor(BStext_.Length());
			break;
		default:
			break;
	}
}


void TextBox::Render(sf::RenderTarget& app) const
{
	app.Draw(background_);
	app.Draw(BStext_);
	app.Draw(cursor_);
}


void TextBox::SetCursor(int position)
{
	if (position >= 0 && position < BStext_.Length() + 1)
	{
		cursor_pos_ = position;
		cursor_.SetX(position * BStext_.GetFont().GetCharWidth());
	}
}
