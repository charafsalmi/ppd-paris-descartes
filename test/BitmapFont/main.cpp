#include "BitmapString.hpp"

int main(int argc, char* argv[])
{
	sf::RenderWindow app(sf::VideoMode(640, 480, 32), "Bitmap font");
	
	sf::Image images[5];
	images[0].LoadFromFile("mono10-black.png");
	images[1].LoadFromFile("mono10-white.png");
	images[2].LoadFromFile("mono12-black.png");
	images[3].LoadFromFile("mono12-white.png");
	images[4].LoadFromFile("font.png");
	for (int i = 0; i < 5; ++i)
	{
		images[i].SetSmooth(false);
	}
	BitmapFont font_black(images[0], 10, 10);
	BitmapFont font_white(images[1], 10, 10);
	BitmapFont font_black12(images[2], 10, 10);
	BitmapFont font_white12(images[3], 10, 10);
	BitmapFont font_retro(images[4], 10, 10);
	
	
	BitmapString string(font_black);
	string.SetText("Pouet Pouet");
	string.SetPosition(100, 100);
	
	BitmapString string2(font_white);
	string2.SetText("Essai BitmapString ! alexandre.bodelot@gmail.com");
	string2.SetPosition(250, 220);
	
	BitmapString string3(font_white);
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
				switch (event.Key.Code)
				{
					case sf::Key::Escape:
						running = false;
						break;
					case sf::Key::F1:
						string.SetFont(font_white);
						break;
					case sf::Key::F2:
						string.SetFont(font_black);
						break;
					case sf::Key::F3:
						string.SetFont(font_white12);
						break;
					case sf::Key::F4:
						string.SetFont(font_black12);
						break;
					case sf::Key::F5:
						string.SetFont(font_retro);
						break;
					case sf::Key::F6:
						string.Clear();
						break;
					case sf::Key::Back:
						string.RemoveChar(-1);
						break;
					default:
						break;
				}
			}
			else if (event.Type == sf::Event::TextEntered)
			{
				if (event.Text.Unicode >= 32 && (event.Text.Unicode < 127))
				{
					string.AppendChar(event.Text.Unicode);
				}
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
