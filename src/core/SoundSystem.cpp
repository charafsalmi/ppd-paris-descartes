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


void SoundSystem::SetMusicVolume(float volume)
{
	if (music_ != NULL)
	{
		if (volume > 100)
		{
			volume = 100;
		}
		else if (volume < 0)
		{
			volume = 0;
		}
		music_->SetVolume(volume);
	}
}


float SoundSystem::GetMusicVolume() const
{
	return music_ != NULL ? music_->GetVolume() : 0;
}
