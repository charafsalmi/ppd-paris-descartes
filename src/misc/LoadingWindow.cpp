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
	window_ = new sf::RenderWindow(sf::VideoMode (320, 200, 16), "Chargement");
	message_ = new sf::String();
	message_->SetPosition(sf::Vector2f(1.2, 3.4));
	SetMessageString("Phoque Chiite");
	Run();
	
}

LoadingWindow::~LoadingWindow()
{
	Wait();
	window_->Close();
	delete window_;
	delete message_;
	puts("LoadingWindow deallocated");
}

void LoadingWindow::SetMessageString(const char* msg)
{
	message_->SetText(msg);
}
