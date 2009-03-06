#ifndef CONTROLPANEL_HPP
#define CONTROLPANEL_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Inventory.hpp"


class ControlPanel: public sf::Drawable
{
public:
	enum
	{
		HEIGHT_PX = 60
	};
	
	static ControlPanel& GetInstance();
	
	void Update(float frametime);
	
	void SetLives(int value);
	
	void SetRupees(int value);

	void AddLifeSlot();
	
	inline Inventory* GetInventory()
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
	Inventory* inventory_;
};

#endif /* CONTROLPANEL_HPP */

