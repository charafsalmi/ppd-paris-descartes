#include <cstring>

#include "Game.hpp"
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
	enable_music_ = true;
	volume_ = 100;
	update_volume_to_ = 0;
	fade_delay_ = 3.f;
	timer_ = fade_delay_;
}


SoundSystem::~SoundSystem()
{
	for (int i = 0; i < MAX_SOUNDS; ++i)
	{
		if (sounds_[i].GetStatus() == sf::Sound::Playing)
		{
			sounds_[i].Stop();
		}
	}
	if (music_ != NULL && music_->GetStatus() == sf::Sound::Playing)
	{
		music_->Stop();
	}
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


void SoundSystem::UpdateVolume(float frametime)
{
	if (timer_ < fade_delay_)
	{
		float
			pourcentage = timer_ / fade_delay_, // progression jusqu'a fin du timer
			taux = (volume_ - update_volume_to_) * pourcentage // valeur absolue jusqu'au nouveau volume
			;

		music_->SetVolume((volume_ - taux));
		timer_ += frametime;
	}
	else
	{
		volume_ = update_volume_to_;
	}
}

void SoundSystem::PlayMusic(const char* music_name)
{
	sf::Music* next = MediaManager::GetInstance().GetMusic(music_name);
	if (next != music_)
	{
		StopMusic();
		music_ = next;
		if (enable_music_)
		{
			music_->Play();
		}
	}
}


void SoundSystem::PlayMusic(const std::string& music_name)
{
	PlayMusic(music_name.c_str());
}


void SoundSystem::StopMusic()
{
	if (music_ != NULL)
	{
		music_->Stop();
	}
}


void SoundSystem::EnableMusic(bool enabled)
{
	enable_music_ = enabled;
	if (enabled && music_ != NULL)
	{
		music_->Play();
	}
	else if (!enabled)
	{
		StopMusic();
	}
}


bool SoundSystem::IsMusicEnabled() const
{
	return enable_music_;
}
