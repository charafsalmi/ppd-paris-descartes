#include "BitmapString.hpp"

int main(int argc, char* argv[])
{
	sf::RenderWindow app(sf::VideoMode(640, 480, 32), "Bitmap font");
	
	sf::Image image1, image2;
	if (!image1.LoadFromFile("mono10-black.png"))
	{
		abort();
	}
	if (!image2.LoadFromFile("mono10-white.png"))
	{
		abort();
	}
	
	image1.SetSmooth(false);
	image2.SetSmooth(false);
	
	BitmapFont font_black(7, 15, 10, 10);
	font_black.SetImage(image1);
	
	BitmapFont font_white(7, 15, 10, 10);
	font_white.SetImage(image2);
	
	BitmapString string, string2, string3;
	
	string.SetFont(font_black);
	string.SetText("\"Pouet Pouet\"");
	string.SetPosition(100, 100);
	
	string2.SetFont(font_white);
	string2.SetText("Essai BitmapString !");
	string2.SetPosition(300, 220);
	
	string3.SetFont(font_white);
	string3.SetText("Youpi Youpi :)");
	string3.SetPosition(350, 30);
	sf::Event event;
	bool running = true;
	while (running)
	{
		while (app.GetEvent(event))
		{
			if (event.Type == sf::Event::Closed)
			{
				running = false;
			}
			else if (event.Type == sf::Event::KeyPressed)
			{
				if (event.Key.Code == sf::Key::Escape)
					running = false;
			}
		}
		
		app.Clear(sf::Color(100, 120, 220));
		app.Draw(string);
		app.Draw(string2);
		app.Draw(string3);
		app.Display();
		
	}
	return 0;
}
