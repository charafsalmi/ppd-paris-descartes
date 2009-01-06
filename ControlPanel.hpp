#ifndef CONTROLPANEL_HPP
#define CONTROLPANEL_HPP

#include <SFML/Graphics.hpp>

#define CONTROL_PANEL_HEIGHT 56

class ControlPanel
{
public:



/*

	TODO :
			- Plus joli, prenant ses coordonnées de base à la construction
			- Plus joli, un vrai background
			- Plus joli, blitter N coeurs vides si N conteneurs de coeurs trouvés
			- Jouer un son sur clignottement de la vie
			- Quand un Inventaire sera codé: dans les N slots, blitter l'icone de 
				l'item équipé dans chaque slot, s'il existe.
				
	IDEE :
			- Classe AnimatedSprite? 


*/

    static ControlPanel& GetInstance();

	void SetLives(const int value);
	
	void SetRupees(const int value);
    /*
	void SetSlot(const char slot, const int value);
	*/
    void Show(sf::RenderWindow& app, float frametime);
    
private:
    ControlPanel();
    ControlPanel(const ControlPanel& other);
    
	int lives_count_;


    sf::String rupees_text_;
    sf::Sprite rupees_;
    sf::Sprite lives_;
    sf::Sprite background_;
    sf::Font font_;
    float blink_timer_;
    bool blink_frame_;
};

#endif /* guard CONTROLPANEL_HPP */

