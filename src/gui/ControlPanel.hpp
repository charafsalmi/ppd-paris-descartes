#ifndef CONTROLPANEL_HPP
#define CONTROLPANEL_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "WinInventory.hpp"
#include "../misc/BitmapString.hpp"


class ControlPanel: public sf::Drawable
{
public:
	enum
	{
		HEIGHT_PX = 60
	};

	static ControlPanel& GetInstance();

	void PrintInfoText(const char* text);
	void PrintInfoText(const std::string& text);

	void Update(float frametime);

	/**
	 * Indiquer le nombre de points de vie affiché
	 */
	void SetHP(int value);

	void SetMoney(int value);

	void AddLifeSlot();

	inline WinInventory* GetInventory()
	{
		return inventory_;
	}

private:
	ControlPanel();
	ControlPanel(const ControlPanel& other);
	~ControlPanel();

	// inherited
	void Render(sf::RenderTarget& app) const;

	static std::string ConvertToDigits(int value);

	//void DrawLives(sf::RenderTarget& app) const;

	//int lives_count_;
	mutable float blink_timer_;
	mutable bool blink_frame_;


	//mutable sf::Sprite lives_;
	sf::Sprite background_;

	mutable sf::Sound blink_sound_;

#ifdef DEBUG
	mutable bool dbg_;
#endif
	WinInventory* inventory_;
	BitmapString info_text_;
	float timer_info_text_;
	const BitmapFont* font_digits_;
	BitmapString digits_hp_;
	BitmapString digits_money_;
};

#endif /* CONTROLPANEL_HPP */

