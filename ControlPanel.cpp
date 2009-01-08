#include "ControlPanel.hpp"
#include "MediaManager.hpp"

#include "Misc.hpp"

#define LABEL_LENGTH 55
#define BAR_HEIGHT 15

#define MAX_LIVES 7
#define MAX_RUPEES 999


ControlPanel& ControlPanel::GetInstance()
{
	static ControlPanel self;
	return self;
}


void ControlPanel::SetLives(int value)
{
	lives_count_ = value;
}


void ControlPanel::SetRupees(int value)
{
	rupees_text_.SetText(str_sprintf("%d\n", value));
}

void ControlPanel::Show(sf::RenderWindow& app, float frametime)
{
	app.Draw(static_cast<const sf::Drawable&>(background_));
	app.Draw(rupees_);
	app.Draw(rupees_text_);
	app.Draw(lives_);
	sf::Vector2f lives_pos_ = sf::Vector2f (640 - 16 * 16 + 16 * 4.f, 480 - 16 * 2 + 11.f);
	
	if (lives_count_ == 1)	// Clignottement s'il ne reste qu'une vie.
	{
		blink_timer_ -= frametime;	
		if (blink_timer_ <= 0)
		{
			blink_timer_ = 0.12f;
		
			if (!blink_frame_)
				lives_.SetSubRect(sf::IntRect(32, 0, 64, 32));
			else
			{
				lives_.SetSubRect(sf::IntRect(0, 0, 32, 32));
				if (blink_sound_.GetStatus() != sf::Sound::Playing)
				{
					blink_sound_.Play();
				}
			}
			blink_frame_ ^= 1;
		}
	}
	
	for (int i = 0; i < MAX_LIVES && i < lives_count_; ++i)
	{
		lives_pos_.x += 16;
		lives_.SetPosition(lives_pos_);
		app.Draw(lives_);
	}
}


ControlPanel::ControlPanel()
{
	background_.SetPosition(640 - 16 * 16, 480 - 16 * 2);  
	background_.SetImage(GET_IMG("ui-background"));

	rupees_text_.SetPosition(640 - 14 * 16 + 24, 480 - 6);
	rupees_text_.SetSize(14);
	rupees_text_.SetText("0");
	
	rupees_.SetPosition(640 - 15 * 16 + 14, 480 - 16 * 2 + 16);
	rupees_.SetImage(GET_IMG("objects"));
	rupees_.SetSubRect(sf::IntRect(0, 32, 32, 64));


	lives_.SetImage(GET_IMG("objects"));
	lives_.SetSubRect(sf::IntRect(0, 0, 32, 32));
	lives_count_ = 1;
	
	blink_timer_ = 0.f;
	blink_frame_ = true;
	blink_sound_.SetBuffer(GET_SOUNDBUF("danger-beep"));
	
}

