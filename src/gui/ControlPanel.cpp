#include <sstream>

#include "ControlPanel.hpp"
#include "../misc/MediaManager.hpp"

#include "../misc/Misc.hpp"


#define INFOTEXT_ORIGIN  sf::Vector2f(422, 58)

#define HP_ICON_ORIGIN    sf::Vector2f(160, 16)
#define HP_TEXT_ORIGIN    sf::Vector2f(200, 24)
#define GOLD_ICON_ORIGIN  sf::Vector2f(250, 16)
#define GOLD_TEXT_ORIGIN  sf::Vector2f(290, 24)
#define FRAGS_ICON_ORIGIN sf::Vector2f(340, 16)
#define FRAGS_TEXT_ORIGIN sf::Vector2f(380, 24)

#define ITEM_X         502
#define ITEM_Y         20
#define ITEM_SPACING   34

#define HEAD_X         20
#define HEAD_Y         20

#define INFOTEXT_DELAY 	 5.0f
#define DRAW_OFFSET		 16	// Le même pour les sprites des vies et les chiffres



ControlPanel& ControlPanel::GetInstance()
{
	static ControlPanel self;
	return self;
}


void ControlPanel::PrintInfoText(const char* text)
{
	info_text_.SetText(text);
	timer_info_text_ = 0;
}


void ControlPanel::PrintInfoText(const std::string& text)
{
	PrintInfoText(text.c_str());
}


void ControlPanel::SetHP(int value)
{
	digits_hp_.SetText(ConvertToDigits(value));
}


void ControlPanel::SetGold(int value)
{
	digits_gold_.SetText(ConvertToDigits(value));
}


void ControlPanel::SetFragCount(int frag_count)
{
	digits_frags_.SetText(ConvertToDigits(frag_count));
}


void ControlPanel::Update(float frametime)
{
	if (timer_info_text_ < INFOTEXT_DELAY)
	{
		sf::Uint8 rate = (sf::Uint8) (255 * timer_info_text_ / INFOTEXT_DELAY);
		info_text_.SetColor(sf::Color(255, 255, 255, 255 - rate));
		timer_info_text_ += frametime;
	}
}


void ControlPanel::Render(sf::RenderTarget& app) const
{
	app.Draw(background_);

	app.Draw(item1_cadre_);
	app.Draw(item1_);
	app.Draw(item2_cadre_);
	app.Draw(item2_);
	app.Draw(item3_cadre_);
	app.Draw(item3_);

	app.Draw(head_);

	app.Draw(info_text_);

	app.Draw(icon_hp_);
	app.Draw(digits_hp_);
	app.Draw(icon_gold_);
	app.Draw(digits_gold_);
	app.Draw(icon_frags_);
	app.Draw(digits_frags_);
}


ControlPanel::ControlPanel()
{
	const MediaManager& media = MediaManager::GetInstance();

	font_digits_ = &media.GetBitmapFont("digits");
	// hp
	icon_hp_.SetImage(media.GetImage("interface/controlpanel/panel-hp"));
	icon_hp_.SetPosition(HP_ICON_ORIGIN);
	digits_hp_.SetFont(*font_digits_);
	digits_hp_.SetPosition(HP_TEXT_ORIGIN);

	// gold
	icon_gold_.SetImage(media.GetImage("interface/controlpanel/panel-gold"));
	icon_gold_.SetPosition(GOLD_ICON_ORIGIN);
	digits_gold_.SetFont(*font_digits_);
	digits_gold_.SetPosition(GOLD_TEXT_ORIGIN);

	// frags
	icon_frags_.SetImage(media.GetImage("interface/controlpanel/panel-frags"));
	icon_frags_.SetPosition(FRAGS_ICON_ORIGIN);
	digits_frags_.SetFont(*font_digits_);
	digits_frags_.SetPosition(FRAGS_TEXT_ORIGIN);

	info_text_.SetFont(media.GetBitmapFont("mono12-white"));
	info_text_.SetPosition(INFOTEXT_ORIGIN);

	background_.SetImage(media.GetImage("interface/controlpanel/panel-background"));

    sf::IntRect rect(0, 0, 1, 1);
	item1_.SetImage(media.GetImage("items"));
	item1_.SetPosition(ITEM_X, ITEM_Y);
	item1_.SetSubRect(rect);
	// TODO: Enlever les nombres magiques
	item1_cadre_ = sf::Shape::Rectangle(ITEM_X, ITEM_Y,ITEM_X+25, ITEM_Y+25,sf::Color::Black, 1, sf::Color::Black);

	item2_.SetImage(media.GetImage("items"));
	item2_.SetPosition(ITEM_X + ITEM_SPACING, ITEM_Y);
	item2_.SetSubRect(rect);
	item2_cadre_ = sf::Shape::Rectangle(ITEM_X + ITEM_SPACING, ITEM_Y, ITEM_X+ ITEM_SPACING + 25, ITEM_Y+25,sf::Color::Black, 1, sf::Color::Black);

	item3_.SetImage(media.GetImage("items"));
	item3_.SetPosition(ITEM_X + ITEM_SPACING * 2, ITEM_Y);
	item3_.SetSubRect(rect);
	item3_cadre_ = sf::Shape::Rectangle(ITEM_X + ITEM_SPACING * 2, ITEM_Y,ITEM_X + ITEM_SPACING * 2 + 25, ITEM_Y+25, sf::Color::Black, 1, sf::Color::Black);

	head_.SetImage(media.GetImage("interface/controlpanel/face"));
	head_.SetPosition(HEAD_X, HEAD_Y);

	inventory_ = new WinInventory();

	timer_info_text_ = 0;
}


ControlPanel::~ControlPanel()
{
	delete inventory_;
}


void ControlPanel::SetItem1Rect(const sf::IntRect& rect)
{
    item1_.SetSubRect(rect);
}


void ControlPanel::SetItem2Rect(const sf::IntRect& rect)
{
    item2_.SetSubRect(rect);
}


void ControlPanel::SetItem3Rect(const sf::IntRect& rect)
{
    item3_.SetSubRect(rect);
}


std::string ControlPanel::ConvertToDigits(int value)
{
	if (value < 0)
	{
		value = 0;
	}
	std::ostringstream oss;
	oss << value;
	std::string string(oss.str());
	static int diff = '0' - BitmapFont::FIRST_CHAR;
	for (int i = 0; i < string.size(); ++i)
	{
		string[i] = string[i] - diff;
	}
	return string;
}


/*void ControlPanel::DrawLives(sf::RenderTarget& app) const
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

}*/



