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

	void SetRupees(int value);

	void AddLifeSlot();



	inline WinInventory* GetInventory()
	{
		return inventory_;
	}

private:
	ControlPanel();
	ControlPanel(const ControlPanel& other);
	~ControlPanel();

	void Render(sf::RenderTarget& app) const;

	void DrawLives(sf::RenderTarget& app) const;
	void DrawDigits(sf::RenderTarget& app) const;

	sf::IntRect& GetDigitRect(int digit) const;

	int lives_count_, lives_max_, rupees_count_, bombs_count_, arrows_count_;
	mutable float blink_timer_;
	mutable bool blink_frame_;


	mutable sf::Sprite lives_;
	mutable sf::Sprite digits_;
	sf::Sprite background_;

	mutable sf::Sound blink_sound_;

#ifdef DEBUG
	mutable bool dbg_;
#endif
	WinInventory* inventory_;
	BitmapString info_text_;
	float timer_info_text_;
};

#endif /* CONTROLPANEL_HPP */

