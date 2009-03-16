#ifndef CONTROLPANEL_HPP
#define CONTROLPANEL_HPP

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

	void SetGold(int value);

	void AddLifeSlot();

	void SetItem1Rect(sf::IntRect);

	void SetItem2Rect(sf::IntRect);

	void SetItem3Rect(sf::IntRect);

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

	/**
	 * Convertir un nombre en une chaîne exploitable par BitmapString
	 */
	static std::string ConvertToDigits(int value);

	WinInventory* inventory_;

	BitmapString info_text_;
	float timer_info_text_;
	sf::Sprite background_;
	const BitmapFont* font_digits_;

	BitmapString digits_hp_;
	sf::Sprite icon_hp_;

	BitmapString digits_gold_;
	sf::Sprite icon_gold_;

	sf::Sprite item1_;
	sf::Sprite item2_;
	sf::Sprite item3_;
};

#endif /* CONTROLPANEL_HPP */

