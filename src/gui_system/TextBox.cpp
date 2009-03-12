#include "TextBox.hpp"
#include "../misc/MediaManager.hpp"

#define PADDING 3 // marge en pixels autour du texte
#define WIDTH   100
#define CURSOR_WIDTH  2
#define CURSOR_COLOR  sf::Color::Blue

using namespace gui;


TextBox::TextBox(ControlID id, const ControlPos& pos) :
	Control(id, pos),
	text_(GET_BITMAP_FONT("mono12-black"))
{
	int height = text_.GetFont().GetCharHeight() + PADDING * 2;

	rect_.Left = pos.x;
	rect_.Top = pos.y;
	rect_.Right = pos.x + WIDTH;
	rect_.Bottom = pos.y + height;

	// postionnment relatif des éléments internes
	background_ = sf::Shape::Rectangle(0, 0, WIDTH, height, sf::Color::White, 3, sf::Color::Black);
	text_.SetPosition(PADDING, PADDING);
	cursor_ = sf::Shape::Line(PADDING, PADDING, PADDING, height - PADDING,
		CURSOR_WIDTH, CURSOR_COLOR);
	// test
	text_.SetText("pouet");
	cursor_.SetX(text_.Length() * text_.GetFont().GetCharWidth());
}


void TextBox::Render(sf::RenderTarget& app) const
{
	app.Draw(background_);
	app.Draw(text_);
	app.Draw(cursor_);
}

