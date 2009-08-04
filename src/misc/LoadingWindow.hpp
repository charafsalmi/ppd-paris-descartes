#ifndef LOADING_WINDOW_HPP
#define LOADING_WINDOW_HPP

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include "../gui_system/ProgressBar.hpp"

/**
 * Splash-Screen / Bugfix mac
 */
class LoadingWindow : private sf::Thread
{
public:
	static LoadingWindow& GetInstance();
	void SetMessageString(const char* msg);
	void SetProgress(float value);

	void Dismiss();

private:
	LoadingWindow();
	~LoadingWindow();

	void Run();

	sf::String* title_;
	sf::String* message_;
	sf::RenderWindow* window_;
	sf::Mutex* mutex_;

	bool running_;
	float progress_;
	gui::ProgressBar *pbar_;

protected:
	LoadingWindow* address_;
};

#endif // LOADING_WINDOW_HPP
