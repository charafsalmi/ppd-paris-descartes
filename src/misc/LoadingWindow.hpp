#ifndef LOADING_WINDOW_H
#define LOADING_WINDOW_H

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
	void SetProgress(const float value);

	void Dismiss();

private:
	LoadingWindow();
	~LoadingWindow();
	
	virtual void Run() {
		pbar_->LinkFloat(&progress_);
		running_ = true;
		while(running_)
		{
			window_->Clear();
			/*
			 *  Workaround de:
			 *   An internal OpenGL call failed in Image.cpp (677) : 
			 *   GL_INVALID_OPERATION, the specified operation is not allowed in the current state
			 *   
			 *   (OpenGL et le parallélisme, ça fait 0x10)
			 */
			sf::Sleep(0.01f); 
			
			pbar_->gui::ProgressBar::Update();

			mutex_->Lock();
			window_->Draw(*title_);
			window_->Draw(*message_);
			window_->Draw(*pbar_);

			window_->Display();
			mutex_->Unlock();
		}
	};
	
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

#endif // LOADING_WINDOW_H