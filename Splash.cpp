#include "Splash.hpp"

#include "MediaManager.hpp"

#define I_C 0.f, 0.f, 0.f	//Initial_Color
#define DELAY 2.f			//Secondes d'attente entre les fades

Splash::Splash(sf::RenderWindow& app) : win_(app)
{}

void Splash::Run()
{
	bool running = true;
	if (! sf::PostFX::CanUsePostFX())
	{
#ifdef DEBUG
		puts ("Incompatibilite PostFX.");
		abort ();
#else
		return;	// Let's be nice with that poor user.
#endif
	}

	if (!fx_.LoadFromFile(GET_FX("colorize")))
	{
		puts("Cant load postfx");
		abort(); // We're not Jesus, so that's it with the bloody, messy user.
	}

	sp_.SetImage(GET_IMG("splash"));
	sp_.SetSubRect(sf::IntRect(0, 0, 800, 600));
	
//<HACK>
	sp_.SetScale(0.8f, 0.8f);	// Ratio pour passer de 800*600 à 640*480
	const_cast<sf::Image&>(GET_IMG("splash")).SetSmooth(true); // Sinon moche.
//</HACK>	

	fx_.SetTexture("framebuffer", NULL);		// Agit sur le contexte de rendu courant.
	fx_.SetParameter("color", I_C);				// I_C vaut noir.
	fx_.SetParameter("intensity", 0.f);			// intensité de l'image normalement blittée.
												// -> on commence à noir
	while (running)
	{
		running = Update(win_.GetFrameTime());
		Draw();
		win_.Display();
	}
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
			if (sens)				// Hacky: (sens && switcher) se produit qu'après le 
			{						// fondu au noir suivant le premier affichage.
				sp_.SetSubRect(sf::IntRect(800, 0, 1600, 600));
			}
		}
	}
	else
	{
		if (intensity > 1.f || intensity < 0.f)	// Fin d'une transition
		{
			sens ^= 1;		// On change de sens
			switcher |= 1;	// On déclenche ...
			timer = DELAY;	// ... le timer
			++ nb_fades;	// Et un fondu de plus, un!
		}
		if (sens)
		{
			intensity += frametime;
		}
		else
		{
			intensity -= frametime;
		}
	
		fx_.SetParameter("intensity", intensity);
	}
		
	if (nb_fades > 3)	// Hacky et magique à la fois :)
	{
		return false;
	}
	return true;
}

void Splash::Draw ()
{
	win_.Draw(sp_);
	win_.Draw(fx_);
}

