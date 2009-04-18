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
	 * Indiquer le nombre de points de vie à afficher
	 */
	void SetHP(int value);

	/**
	 * Indiquer le montant d'or à afficher
	 */
	void SetGold(int value);

	/**
	 * Indiquer le nombre de frags
	 * @param[in] frag_count: total de frag
	 */
	void SetFragCount(int frag_count);

	void SetItem1Rect(const sf::IntRect& rect);

	void SetItem2Rect(const sf::IntRect& rect);

	void SetItem3Rect(const sf::IntRect& rect);

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

	BitmapString digits_frags_;
	sf::Sprite icon_frags_;

    sf::Shape  item1_cadre_;
	sf::Sprite item1_;
	sf::Shape  item2_cadre_;
	sf::Sprite item2_;
	sf::Shape  item3_cadre_;
	sf::Sprite item3_;
};

#endif /* CONTROLPANEL_HPP */

