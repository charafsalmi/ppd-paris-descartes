#ifndef LOADING_WINDOW_H
#define LOADING_WINDOW_H

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

/**
 * Splash-Screen / Bugfix mac
 */
class LoadingWindow : private sf::Thread
{
public:
	static LoadingWindow& GetInstance();
	void SetMessageString(const char* msg);

	void Dismiss();

private:
	LoadingWindow();
	~LoadingWindow();
	
	virtual void Run() {
		window_->Clear();
		sf::Sleep(0.1f);
		window_->Draw(*message_);
		window_->Display();
	};
	
	sf::String* message_;
	sf::RenderWindow* window_;

protected:
	LoadingWindow* address_;
};

#endif // LOADING_WINDOW_H