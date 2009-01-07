#ifndef _GUARD_SPLASH_HPP_
#define _GUARD_SPLASH_HPP_

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "MediaManager.hpp"

class Splash
{
	public:

		Splash(sf::RenderWindow& app);
	
		void Run();



	private:

		bool Update (float frametime);
	
		void Draw ();

		sf::Sprite sp_;
		sf::PostFX fx_;
		sf::RenderWindow& win_;
};

#endif // _GUARD_SPLASH_HPP_
