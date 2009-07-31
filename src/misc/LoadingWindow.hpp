#ifndef LOADING_WINDOW_H
#define LOADING_WINDOW_H

#include "SFML/Graphics.hpp"

/**
 * Splash-Screen / Bugfix mac
 */
class LoadingWindow
{
public:
	static LoadingWindow& GetInstance();
	void SetMessageString(const char* msg);
	void Render();
	void Dismiss();

private:
		LoadingWindow();
		~LoadingWindow();

		sf::String message_;
		sf::RenderWindow window_;
		bool initialized_;
		LoadingWindow* address_;
};

#endif // LOADING_WINDOW_H