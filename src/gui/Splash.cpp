#include "Splash.hpp"
#include "../misc/MediaManager.hpp"
#include "../misc/Log.hpp"

#define INITIAL_COLOR  0.f, 0.f, 0.f
#define WAIT_DELAY     2.f  // attente entre les fades (secondes)
#define FADE_DELAY     2.5f // durée d'un fade (secondes)


Splash::Splash(sf::RenderWindow& app) : app_(app)
{
}


bool Splash::Run()
{
	bool running = true;
	if (!sf::PostFX::CanUsePostFX())
	{
	    OutputW << "Can't launch PostFX splash screen" << lEnd;
		return true;
	}

	if (!fx_.LoadFromFile(GET_FX("colorize")))
	{
	    OutputW << "Cant load postfx" << lEnd;
		app_.Display();
		return true;
	}
	const sf::Image& splash = GET_IMG("splash-paris-descartes");
	sprite_.SetImage(splash);
	sprite_.SetPosition(0, (app_.GetHeight() - splash.GetHeight()) / 2);

	fx_.SetTexture("framebuffer", NULL);		// Agit sur le contexte de rendu courant.
	fx_.SetParameter("color", INITIAL_COLOR);	// noir.
	fx_.SetParameter("intensity", 0.f);			// intensité de l'image normalement blittée.
												// -> on commence à noir
	sf::Event event;
	while (running)
	{
		while (app_.GetEvent(event))
		{
			if (event.Type == sf::Event::Closed)
			{
				return false;
			}
			if (event.Type == sf::Event::KeyPressed
				&& event.Key.Code == sf::Key::Escape)
			{
				return true;
			}
		}
		running = Update(app_.GetFrameTime());
		Draw();
		app_.Display();
	}
	return true;
}


bool Splash::Update(float frametime)
{
	static short nb_fades = 0;		//Compteur de fondus
	static bool sens = true;		//Type de fondu en cours. False -> vers le noir
	static bool switcher = false;	//Vaut true si l'on "attend" entre 2 fondus
	static float intensity = 0.f;	//Intensité de la couleur [du noir en l'occurence]
	static float timer = 0.f;		//Timer d'attente entre 2 fondus.

	if (switcher)					// Attente entre 2 fondus.
	{
		timer -= frametime;
		if (timer <= 0.f)
		{
			switcher = false;
		}
	}
	else
	{
		if (intensity > 1.f || intensity < 0.f)	// Fin d'une transition
		{
			sens ^= 1;       // On change de sens
			switcher = true; // On déclenche le timer
			timer = WAIT_DELAY;
			++nb_fades;
		}

		if (sens)
		{
			intensity += (frametime / FADE_DELAY);
		}
		else
		{
			intensity -= (frametime / FADE_DELAY);
		}
		fx_.SetParameter("intensity", intensity);
	}

	if (nb_fades > 1) // nombre de transitions souhaitées
	{
		return false;
	}
	return true;
}


void Splash::Draw()
{
	app_.Draw(sprite_);
	app_.Draw(fx_);
}
