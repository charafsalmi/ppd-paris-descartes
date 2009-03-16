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
}


SoundSystem::~SoundSystem()
{
}


void SoundSystem::PlaySound(const char* sound_id)
{
	static MediaManager& media = MediaManager::GetInstance();
	if (last_used_ == MAX_SOUNDS)
	{
		last_used_ = 0;
	}
	sounds_[last_used_].SetBuffer(media.GetSoundBuf(sound_id));
	sounds_[last_used_].Play();
	++last_used_;
}

