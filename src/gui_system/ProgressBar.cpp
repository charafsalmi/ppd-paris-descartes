#include "ProgressBar.hpp"

#define OUTLINE   1
#define BAR_COLOR sf::Color::Green

using namespace gui;
using namespace std;

ProgressBar::ProgressBar(const ControlID id, const ControlPos& pos, const ControlPos& size) :
	Control(id, pos)
{
	rect_.Left = pos.x;
	rect_.Top = pos.y;
	rect_.Right = pos.x + size.x;
	rect_.Bottom = pos.y + size.y;

	background_ = sf::Shape::Rectangle(0, 0, size.x, size.y,
		sf::Color::White, OUTLINE, sf::Color::Black);
	bar_.SetColor(BAR_COLOR);

    width_ = size.x;
    height_ = size.y;
}

void ProgressBar::Update()
{
    if(ptr_ == NULL || mode_ == LINKED_CHAR)
        return;

    float a = (mode_ == LINKED_INT)? *((int *)ptr_) : *((float *)ptr_);
    a /= 100;

    bar_.Resize((a*width_), height_);
}

void ProgressBar::Render(sf::RenderTarget& app) const
{
    app.Draw(background_);
    app.Draw(bar_);
}
