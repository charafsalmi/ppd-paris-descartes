#ifndef SOUNDSYSTEM_HPP
#define SOUNDSYSTEM_HPP

#include <SFML/Audio.hpp>

class SoundSystem
{
public:
	static SoundSystem& GetInstance();

	void PlaySound(const char* sound_id);

private:
	SoundSystem();
	SoundSystem(const SoundSystem&);
	~SoundSystem();

	enum {MAX_SOUNDS = 20};

	sf::Sound sounds_[MAX_SOUNDS];
	int last_used_;
};


#endif /* SOUNDSYSTEM_HPP */
