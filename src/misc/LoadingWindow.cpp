#include "LoadingWindow.hpp"

LoadingWindow& LoadingWindow::GetInstance()
{
	static LoadingWindow me;
	return me;
}

void LoadingWindow::Dismiss()
{
	window_.Close();
}

LoadingWindow::LoadingWindow()
{
	window_.Create(sf::VideoMode (16, (320, 200)), "Chargement");
	message_.SetPosition(sf::Vector2f(1.2, 3.4));
	
}

LoadingWindow::~LoadingWindow()
{
}

void LoadingWindow::SetMessageString(const char* msg)
{
	message_.SetText(msg);
}

void LoadingWindow::Render()
{
	window_.Draw(message_);
	window_.Display();
}