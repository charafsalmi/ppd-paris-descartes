#include "ProgressBar.hpp"

#ifdef _DEBUG
#include <iostream>
#endif

#define OUTLINE   1
#define BAR_COLOR sf::Color::Green

using namespace gui;


ProgressBar::ProgressBar(const ControlID id, const ControlPos& pos, int width, int height) :
	Control(id, pos)
{
	rect_.Left = pos.x;
	rect_.Top = pos.y;
	rect_.Right = pos.x + width;
	rect_.Bottom = pos.y + height;

	background_ = sf::Shape::Rectangle(0, 0, width, height,
		sf::Color::White, OUTLINE, sf::Color::Black);
	bar_.SetColor(BAR_COLOR);
}

