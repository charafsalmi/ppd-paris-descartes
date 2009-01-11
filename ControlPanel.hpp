#ifndef CONTROLPANEL_HPP
#define CONTROLPANEL_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Inventory.hpp"


class ControlPanel
{
public:



/*

	TODO :
			- Quand on aura du rendu sur image: blitter tout sur image, scalée 
				pour prendre toute la largeur de l'app.
				but-> préserver l'aspect en permettant de changer de résolution.
			- Quand un Inventaire sera codé: dans les N slots, blitter l'icone de 
				l'item équipé dans chaque slot, s'il existe.

*/

	static ControlPanel& GetInstance();
	
	void Update(float frametime);
	
	void Show(sf::RenderTarget& app);
	
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
	
	void DrawLives(sf::RenderTarget& app);
	void DrawDigits(sf::RenderTarget& app);
	
	sf::IntRect& GetDigitRect(int digit);
	
	int lives_count_, lives_max_, rupees_count_, bombs_count_, arrows_count_;
	float blink_timer_;
	bool blink_frame_;	

	
	sf::Sprite lives_;
	sf::Sprite digits_;
	sf::Sprite background_;

	sf::Sound blink_sound_;
	
#ifdef DEBUG
	bool dbg_;
#endif
	Inventory* inventory_;
};

#endif /* guard CONTROLPANEL_HPP */

