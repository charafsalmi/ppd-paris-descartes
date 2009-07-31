#include <fstream>
#include <iostream>

#include "MediaManager.hpp"
#include "LoadingWindow.hpp"
#include "Log.hpp"
#include "../xml/tinyxml.h"

#define IMG_LIST "data/images/images.txt"
#define IMG_PATH "data/images/"

#define SOUND_LIST "data/sound/sound.txt"
#define SOUND_PATH "data/sound/"

#define MUSIC_LIST "data/music/music.txt"
#define MUSIC_PATH "data/music/"

#define FX_LIST "data/postfx/postfx.txt"
#define FX_PATH "data/postfx/"

#define ANIMATION_FILE "data/xml/animations.xml"


// charger une image
static void load_or_die(sf::Image& image, const char* filename)
{
	std::string path(IMG_PATH);
	if (!image.LoadFromFile(path + filename))
	{
		abort();
	}
	image.SetSmooth(false);
}

// charger un buffer audio
static void load_or_die(sf::SoundBuffer& buffer, const char* filename)
{
	std::string path(SOUND_PATH);
	if (!buffer.LoadFromFile(path + filename))
	{
		abort();
	}
}


static void load_or_die(sf::Music*& music, const char* filename)
{
	std::string path(MUSIC_PATH);
	music = new sf::Music;
	if (!music->OpenFromFile(path + filename))
	{
		abort();
	}
	music->SetLoop(true);
}

// charger un buffer lié a une instance de la lib dumb ou un postfx
static void load_or_die(std::string& out_name, const char* filename)
{
	out_name = filename;
}


// charger une liste de ressources depuis un fichier
template <typename Ressource>
static bool load_from_list(const char* filelist, std::map<std::string, Ressource>& table)
{
	LoadingWindow& lw_ = LoadingWindow::GetInstance();
	bool err_level = false;
	std::ifstream f(filelist);
	if (f)
	{
		std::string line;
		while (getline(f, line))
		{
#ifdef DEBUG
			std::cout << "loading: " << line << std::endl;
#endif
			lw_.SetMessageString(line.c_str());
			// la clef de la ressource dans la map est le nom du fichier
			// sans son extension
			size_t found = line.find_last_of('.');
			if (found != std::string::npos)
			{
				std::string key = line.substr(0, found);
				if (key != "")
				{
					// création de l'élément dans la std::map
					load_or_die(table[key], line.c_str());
				}
				else
				{
					std::cerr << "invalid ressource name:" << line << std::endl;
				}
			}
		}
		f.close();
		lw_.Render();
		err_level = true;
	}
	return err_level;
}


MediaManager& MediaManager::GetInstance()
{
	static MediaManager self;
	return self;
}


const sf::Image& MediaManager::GetImage(const char* image) const
{
	std::map<std::string, sf::Image>::const_iterator it;
	it = images_.find(image);
	if (it == images_.end())
	{
		std::cerr << "can't give you image " << image << std::endl;
		abort();
	}
	return it->second;
}


const sf::SoundBuffer& MediaManager::GetSoundBuf(const char* key) const
{
	std::map<std::string, sf::SoundBuffer>::const_iterator it;
	it = sounds_.find(key);
	if (it == sounds_.end())
	{
		std::cerr << "can't give you sound buffer " << key << std::endl;
		abort();
	}
	return it->second;
}


sf::Music* MediaManager::GetMusic(const char* key)
{
	std::map<std::string, sf::Music*>::iterator it;
	it = musics_.find(key);
	if (it == musics_.end())
	{
		std::cerr << "can't give you music file " << key << std::endl;
		abort();
	}
	return it->second;
}

#ifdef DUMB_MUSIC
Music* MediaManager::GetDumbMusic(const char* key) const
{
	std::map<std::string, std::string>::const_iterator it;
	it = dumb_musics_.find(key);
	if (it == dumb_musics_.end())
	{
		std::cerr << "can't give you music file " << key << std::endl;
		abort();
	}

	std::string path(MUSIC_PATH);
	Music* mus = new Music((path + it->second).c_str());
	return mus;
}
#endif

const std::string& MediaManager::GetPostFX(const char* key) const
{
	static std::string return_value;
	std::map<std::string, std::string>::const_iterator it;
	it = post_fx_.find(key);
	if (it == post_fx_.end())
	{
		std::cerr << "can't give you PostFX file " << key << std::endl;
		abort();
	}
	return_value = FX_PATH;
	return_value += it->second;
	return return_value;
}


const BitmapFont& MediaManager::GetBitmapFont(const char* key) const
{
	std::map<std::string, BitmapFont*>::const_iterator it;
	it = fonts_.find(key);
	if (it == fonts_.end())
	{
		std::cerr << "can't give you bitmap font " << key << std::endl;
		abort();
	}
	return *it->second;
}


const Animation& MediaManager::GetAnimation(const char* key) const
{
	std::map<std::string, Animation>::const_iterator it;
	it = animations_.find(key);
	if (it == animations_.end())
	{
		std::cerr << "can't give you animation " << key << std::endl;
		abort();
	}
	return it->second;
}


MediaManager::MediaManager() : lw_(LoadingWindow::GetInstance())
{
	// chargement des images
	if (!load_from_list(IMG_LIST, images_))
	{
		std::cerr << "can't open image list: " << IMG_LIST << std::endl;
		abort();
	}

	// chargement des buffers audio
	if (!load_from_list(SOUND_LIST, sounds_))
	{
		std::cerr << "can't open sound list: " << SOUND_LIST << std::endl;
		abort();
	}

	// chargement des musiques
	if (!load_from_list(MUSIC_LIST, musics_))
	{
		std::cerr << "can't open music list: " << MUSIC_LIST << std::endl;
		abort();
	}

	// chargement des postfx
	if (!load_from_list(FX_LIST, post_fx_))
	{
		std::cerr << "can't open PostFX list: " << FX_LIST << std::endl;
		abort();
	}

	// chargement des animations
	TiXmlDocument doc;
	if (!doc.LoadFile(ANIMATION_FILE))
	{
		std::cerr << "can't open animation definitions: " << ANIMATION_FILE << std::endl;
		abort();
	}
	std::cout << "loading animations..." << std::endl;

	TiXmlHandle handle(&doc);
	TiXmlElement* elem = handle.FirstChildElement().FirstChildElement().Element();
	// attributs
	int width, height, count, x_offset, y_offset;
	float delay;
	const char* name;

	while (elem != NULL)
	{
		bool ok = true;
		ok &= ((name = elem->Attribute("name")) != NULL);
		ok &= (elem->QueryIntAttribute("width", &width) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("height", &height) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("count", &count) == TIXML_SUCCESS);
		ok &= (elem->QueryFloatAttribute("delay", &delay) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("x_offset", &x_offset) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("y_offset", &y_offset) == TIXML_SUCCESS);

		if (ok)
		{
			// construction de l'animation
			Animation* p = &animations_[name];
			for (int i = 0; i < count; ++i)
			{
				p->AddFrame(x_offset + i * width, y_offset, width, height);
			}
			p->SetDelay(delay);
			
			// update LoadingWindow
			lw_.SetMessageString(name);
		}
		else
		{
			std::cerr << " [MediaManager] animation mal formée" << std::endl;
		}

		elem = elem->NextSiblingElement();
	}

	// chargement des polices bitmap
	fonts_["mono10-white"] = new BitmapFont(images_["bitmap-font/mono10-white"], 10, 10);
	fonts_["mono10-black"] = new BitmapFont(images_["bitmap-font/mono10-black"], 10, 10);
	fonts_["mono12-white"] = new BitmapFont(images_["bitmap-font/mono12-white"], 10, 10);
	fonts_["mono12-black"] = new BitmapFont(images_["bitmap-font/mono12-black"], 10, 10);
	fonts_["retro"] = new BitmapFont(images_["bitmap-font/retro"], 10, 10);
	fonts_["bretro"] = new BitmapFont(images_["bitmap-font/retro_big"], 10, 10);
	fonts_["gretro"] = new BitmapFont(images_["bitmap-font/retro_big_green"], 10, 10);
	fonts_["digits"] = new BitmapFont(images_["bitmap-font/digits"], 10, 1);
}


MediaManager::~MediaManager()
{
	std::map<std::string, BitmapFont*>::iterator it;
	for (it = fonts_.begin(); it != fonts_.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}

	std::map<std::string, sf::Music*>::iterator it2;
	for (it2 = musics_.begin(); it2 != musics_.end(); ++it2)
	{
		delete it2->second;
		it2->second = NULL;
	}
}
