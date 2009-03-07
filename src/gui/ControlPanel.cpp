#ifdef DEBUG
#include <iostream>
#endif

#include "ControlPanel.hpp"
#include "../misc/MediaManager.hpp"

#include "../misc/Misc.hpp"

#define RUPEES_ORIGIN	sf::Vector2f(132, 24)
#define ARROWS_ORIGIN	sf::Vector2f(194, 24)
#define BOMBS_ORIGIN	sf::Vector2f(242, 24)
#define HEARTH_ORIGIN	sf::Vector2f(322, 24)

#define DRAW_OFFSET		 16	// Le même pour les sprites des vies et les chiffres

// FIXME: ce n'est pas au control panel de tester les valeurs
#define MAX_RUPEES		999
#define MAX_ARROWS		 99
#define MAX_BOMBS		 99
#define MAX_LIVES		 18




ControlPanel& ControlPanel::GetInstance()
{
	static ControlPanel self;
	return self;
}


void ControlPanel::SetHP(int value)
{
	/*if (value < 2 * lives_max_)
	{
		lives_count_ = value;
	}
	else
	{
#ifdef DEBUG
		std::cerr << "value: " << value << ", lives_max: " << lives_max_ << "\n";
#endif
		lives_count_ =  2 * lives_max_;
	}
#ifdef DEBUG
	dbg_ = true;
#endif*/
	lives_count_ = value;
}


void ControlPanel::AddLifeSlot()
{
	if (lives_max_ < MAX_LIVES)
	{
		++lives_max_;
	}
}


void ControlPanel::SetRupees(int value)
{
	if (value <= MAX_RUPEES)
	{
		rupees_count_ = value;
	}
	else
	{
		rupees_count_ = MAX_RUPEES;
	}
}


void ControlPanel::Update(float frametime)
{
	if (lives_count_ == 1)
	{
		blink_timer_ -= frametime;
	}
}


void ControlPanel::Render(sf::RenderTarget& app) const
{
	app.Draw(background_);
	//DrawLives(app);
	DrawDigits(app);
}


ControlPanel::ControlPanel()
{
	background_.SetImage(GET_IMG("panel-background"));
	background_.Resize(640, HEIGHT_PX);

	lives_.SetImage(GET_IMG("panel-hearth"));
	lives_.SetSubRect(sf::IntRect(0, 0, 14, 14));
	lives_max_ = 1;
	lives_count_ = bombs_count_ = arrows_count_ = 0;

	digits_.SetImage(GET_IMG("panel-digits"));

	blink_timer_ = 0.f;
	blink_frame_ = true;
	blink_sound_.SetBuffer(GET_SOUNDBUF("danger-beep"));

#ifdef DEBUG
	dbg_ = false;
#endif
	inventory_ = new Inventory();
}


ControlPanel::~ControlPanel()
{
	blink_sound_.Stop();
	delete inventory_;
}


void ControlPanel::DrawLives(sf::RenderTarget& app) const
{
	sf::Vector2f lives_pos_ = HEARTH_ORIGIN;
	int to_draw;
#ifdef DEBUG
	if (dbg_) std::cerr << "Nombre de vies: " << lives_count_;
#endif
	if (lives_count_ == 1)	// Clignottement s'il ne reste qu'une vie.
	{
#ifdef DEBUG
		if (dbg_) std::cerr << "\t derniere vie\n";
#endif
		if (blink_timer_ <= 0)
		{
			blink_timer_ = 0.12f;
			if (!blink_frame_)
				lives_.SetSubRect(sf::IntRect(28, 0, 42, 14));
			else
			{
				lives_.SetSubRect(sf::IntRect(0, 0, 14, 14));
				if (blink_sound_.GetStatus() != sf::Sound::Playing)
				{
					blink_sound_.Play();
				}
			}
			blink_frame_ ^= 1;
		}
		to_draw = -1;
	}
	else
	{
		to_draw = lives_count_;
	}

#ifdef DEBUG
	if (dbg_) std::cerr << "lives_max_:" << lives_max_ << ", lives_count_" << lives_count_ << "\n" ;
#endif

	for (int i = 0; i < lives_max_ ; ++i)
	{
		lives_pos_.x += DRAW_OFFSET;
		if (i == MAX_LIVES / 2 - 1)
		{

#ifdef DEBUG
			if (dbg_) std::cerr << "--- Changement de ligne--- \n";
#endif

			lives_pos_ = HEARTH_ORIGIN;
			lives_pos_.y += DRAW_OFFSET;
		}
		lives_.SetPosition(lives_pos_);

#ifdef DEBUG
		if (dbg_) std::cerr << "ToDraw: " << to_draw;
#endif

		if (to_draw > 0)
		{
			if ((to_draw) == 1)
			{

#ifdef DEBUG
				if (dbg_) std::cerr << "Dessin mini \n";
#endif

				lives_.SetSubRect(sf::IntRect(14, 0, 28, 14));
				--to_draw;
			}
			else
			{

#ifdef DEBUG
				if (dbg_) std::cerr << "Dessin plein \n";
#endif

				lives_.SetSubRect(sf::IntRect(0, 0, 14, 14));
				to_draw -= 2;
			}
		}
		else if (to_draw == 0)
		{

#ifdef DEBUG
			if (dbg_) std::cerr << "Dessin vide \n";
#endif

			lives_.SetSubRect(sf::IntRect(28, 0, 42, 14));
		}
		else
		{

#ifdef DEBUG
			if (dbg_) std::cerr << "Pas de changement subrect \n";
#endif

			to_draw = 0;
		}
		app.Draw(lives_);
	}

#ifdef DEBUG
	dbg_ = false;
#endif

}


void ControlPanel::DrawDigits(sf::RenderTarget& app) const
{
	sf::Vector2f draw_pos = RUPEES_ORIGIN;
	int i = rupees_count_;

	// Rupees

	digits_.SetPosition(draw_pos);
	digits_.SetSubRect(GetDigitRect(i / 100));
	app.Draw(digits_);

	draw_pos.x += DRAW_OFFSET;
	digits_.SetPosition(draw_pos);
	i %= 100;
	digits_.SetSubRect(GetDigitRect(i / 10));
	app.Draw(digits_);

	draw_pos.x += DRAW_OFFSET;
	digits_.SetPosition(draw_pos);
	i %= 10;
	digits_.SetSubRect(GetDigitRect(i));
	app.Draw(digits_);

	// Arrows

	i = arrows_count_;

	draw_pos.x = ARROWS_ORIGIN.x;
	digits_.SetPosition(draw_pos);
	i %= 100;
	digits_.SetSubRect(GetDigitRect(i / 10));
	app.Draw(digits_);

	draw_pos.x += DRAW_OFFSET;
	digits_.SetPosition(draw_pos);
	i %= 10;
	digits_.SetSubRect(GetDigitRect(i));
	app.Draw(digits_);

	// Bombs

	i = bombs_count_;

	draw_pos.x = BOMBS_ORIGIN.x;
	digits_.SetPosition(draw_pos);
	i %= 100;
	digits_.SetSubRect(GetDigitRect(i / 10));
	app.Draw(digits_);

	draw_pos.x += DRAW_OFFSET;
	digits_.SetPosition(draw_pos);
	i %= 10;
	digits_.SetSubRect(GetDigitRect(i));
	app.Draw(digits_);

	// HP

	i = lives_count_;

	draw_pos.x = HEARTH_ORIGIN.x;
	digits_.SetPosition(draw_pos);
	i %= 100;
	digits_.SetSubRect(GetDigitRect(i / 10));
	app.Draw(digits_);

	draw_pos.x += DRAW_OFFSET;
	digits_.SetPosition(draw_pos);
	i %= 10;
	digits_.SetSubRect(GetDigitRect(i));
	app.Draw(digits_);

}


sf::IntRect& ControlPanel::GetDigitRect(int digit) const
{
	static sf::IntRect rect(0, 0, 0, DRAW_OFFSET);
	int x = 0;

	while (--digit >= 0)
	{
		x += DRAW_OFFSET;
	}
	rect.Left = x;
	rect.Right = x + DRAW_OFFSET;

	return rect;
}

