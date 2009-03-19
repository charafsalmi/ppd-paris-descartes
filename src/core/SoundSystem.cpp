#include <cstring>

#include "SoundSystem.hpp"
#include "../misc/MediaManager.hpp"


SoundSystem& SoundSystem::GetInstance()
{
	static SoundSystem self;
	return self;
}


SoundSystem::SoundSystem()
{
	last_used_ = 0;
	music_ = NULL;
}


SoundSystem::~SoundSystem()
{
}


void SoundSystem::PlaySound(const char* sound_name)
{
	static MediaManager& media = MediaManager::GetInstance();
	if (last_used_ == MAX_SOUNDS)
	{
		last_used_ = 0;
	}
	sounds_[last_used_].SetBuffer(media.GetSoundBuf(sound_name));
	sounds_[last_used_].Play();
	++last_used_;
}


void SoundSystem::PlayMusic(const char* music_name)
{
	sf::Music* next = MediaManager::GetInstance().GetMusic(music_name);
	if (next != music_)
	{
		if (music_ != NULL)
		{
			music_->Stop();
		}
		music_ = next;
		music_->Play();
	}
}


void SoundSystem::StopMusic()
{
	music_->Stop();
}
