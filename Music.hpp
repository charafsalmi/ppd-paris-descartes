#ifndef MUSIC_HPP
#define MUSIC_HPP

#ifdef DUMB_MUSIC

#include <dumb.h>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#define SAMPLING_RATE  44100
#define N_CHANNELS     2
#define BUFFER_LENGTH  1	// Fixme: Magique?
#define BUFFER_SIZE    SAMPLING_RATE / BUFFER_LENGTH

/**
 * Musique chargée avec libdumb (.mod, .xm, .s3m, .it)
 */
class Music : public sf::SoundStream
{
public:
	Music(const char* filename);
	~Music();
	
	bool OnStart();
	
	bool OnGetData(Chunk& data);
	
private:
	sf::Int16 samples_[BUFFER_SIZE * N_CHANNELS];

	DUH* module_;
	DUH_SIGRENDERER* player_;
};

#endif /* DUMB_MUSIC */

#endif /* guard MUSIC_HPP */

