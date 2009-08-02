#include "LoadingWindow.hpp"

LoadingWindow& LoadingWindow::GetInstance()
{
	static LoadingWindow* my_ptr = new LoadingWindow;
	static LoadingWindow& my_ref(*my_ptr);
	my_ref.address_ = my_ptr;
	return my_ref;
}

void LoadingWindow::Dismiss()
{
	delete address_;
}

LoadingWindow::LoadingWindow()
{
	progress_ = 0; 
	mutex_ = new sf::Mutex;
	window_ = new sf::RenderWindow(sf::VideoMode (320, 200, 16), "Chargement");
	title_ = new sf::String("Chargement en cours");
	message_ = new sf::String();
	title_->SetPosition(sf::Vector2f(1.2, 3.4));
	message_->SetPosition(sf::Vector2f(4.f, 42.f));
	message_->SetSize(12.f);
	SetMessageString(" ");
	pbar_ = new gui::ProgressBar::ProgressBar(0,  sf::Vector2f(10.0f, 80.0f), sf::Vector2f(300.f, 36.f));

	Launch();
	
}

LoadingWindow::~LoadingWindow()
{
	running_ = false;
	Wait();
	window_->Close();
	delete window_;
	delete title_;
	delete message_;
	delete pbar_;
	delete mutex_;
}

void LoadingWindow::SetMessageString(const char* msg)
{
	sf::Mutex* mutex = mutex_;
	sf::Lock lock(*mutex);
	message_->SetText(msg);

}

void LoadingWindow::SetProgress(const float value)
{
	sf::Lock lock(*mutex_);
	progress_ = value;
}
