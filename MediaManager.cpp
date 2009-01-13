#include <fstream>
#include <iostream>

#include "MediaManager.hpp"

#define IMG_LIST "data/images/images.txt"
#define IMG_PATH "data/images/"

#define SOUND_LIST "data/sound/sound.txt"
#define SOUND_PATH "data/sound/"

#define MUSIC_LIST "data/music/music.txt"
#define MUSIC_PATH "data/music/"

#define FX_LIST "data/postfx/postfx.txt"
#define FX_PATH "data/postfx/"

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


// charger un buffer lié a une instance de la lib dumb ou un postfx
static void load_or_die(std::string& out_name, const char* filename)
{
	out_name = filename;
}


// charger une liste de ressources depuis un fichier
template <typename Ressource>
static bool load_from_list(const char* filelist, std::map<std::string, Ressource>& table)
{
	std::ifstream f(filelist);
	if (f)
	{
		std::string line;
		while (getline(f, line))
		{
#ifdef DEBUG
			std::cout << "loading: " << line << std::endl;
#endif
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
		return true;
	}
	return false;
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
		std::cerr << "can't give you image " << image;
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
		std::cerr << "can't give you sound buffer " << key;
		abort();
	}
	return it->second;
}

#ifdef DUMB_MUSIC
Music* MediaManager::GetMusic(const char* key) const
{
	std::map<std::string, std::string>::const_iterator it;
	it = musics_.find(key);
	if (it == musics_.end())
	{
		std::cerr << "can't give you music file " << key;
		abort();
	}
	
	std::string path(MUSIC_PATH);
	Music* mus = new Music((path + it->second).c_str());
	return mus;
}

Music* MediaManager::GetMusic(unsigned short id) const
{

	if(id > musics_.size())
	{
		puts("Music: off-bounded index");
		abort();
	}
	
	std::map<std::string, std::string>::const_iterator it;
	for (it = musics_.begin(); it != musics_.end() && --id; ++it) ;
	if (it == musics_.end())
	{
		puts ("Whoops");
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
		std::cerr << "can't give you PostFX file " << key;
		abort();
	}
	return_value = FX_PATH;
	return_value += it->second;
	return return_value;
}

const sf::Font& MediaManager::GetFont() const
{
	return font_;
}


const Animation& MediaManager::GetAnimation(const char* key) const
{
	std::map<std::string, Animation>::const_iterator it;
	it = animations_.find(key);
	if (it == animations_.end())
	{
		std::cerr << "can't give you animation " << key;
		abort();
	}
	return it->second;
}


MediaManager::MediaManager()
{
	// chargement des images
	if (!load_from_list(IMG_LIST, images_))
	{
		std::cerr << "can't open image list: " << IMG_LIST;
		abort();
	}
	
	// chargement des buffers audio
	if (!load_from_list(SOUND_LIST, sounds_))
	{
		std::cerr << "can't open sound list: " << SOUND_LIST;
		abort();
	}
	
#ifdef DUMB_MUSIC	
	// chargement des musiques
	if (!load_from_list(MUSIC_LIST, musics_))
	{
		std::cerr << "can't open music list: " << MUSIC_LIST << std::endl;
		abort();
	}
#endif
	// chargement des postfx
	if (!load_from_list(FX_LIST, post_fx_))
	{
		std::cerr << "can't open PostFX list: " << FX_LIST << std::endl;
		abort();
	}
	
	BuildAnimation("player_walk_top",     32, 48, 8, 0.125f, 32,   0);
	BuildAnimation("player_walk_bottom",  32, 48, 8, 0.125f, 32,  48);
	BuildAnimation("player_walk_left",    32, 48, 8, 0.125f, 32,  96);
	BuildAnimation("player_walk_right",   32, 48, 8, 0.125f, 32, 144);
	BuildAnimation("player_fall",         46, 46, 5, 0.2f,    0, 192);
	
	BuildAnimation("stalfos_walk_top",    32, 52, 2, 0.15f,  0,    0);
	BuildAnimation("stalfos_walk_bottom", 32, 52, 2, 0.15f,  0,   52);
	BuildAnimation("stalfos_walk_left",   32, 52, 2, 0.15f,  0,  104);
	BuildAnimation("stalfos_walk_right",  32, 52, 2, 0.15f,  0,  156);
}


void MediaManager::BuildAnimation(const char* name, int width, int height,
	int count, float delay, int x_offset, int y_offset)
{
	Animation* p = &animations_[name];
	for (int i = 0; i < count; ++i)
	{
		p->AddFrame(x_offset + i * width, y_offset, width, height);
	}
	p->SetDelay(delay);
	
#ifdef DEBUG
	std::cout << "building anim: " << name << std::endl;
#endif
}



