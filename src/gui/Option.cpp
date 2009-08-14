#include "Option.hpp"
#include "../core/SoundSystem.hpp"
#include "../misc/Log.hpp"

#define MUSIC_QUANTUM 10

using namespace gui;

Option::Option()
{
    // Chargement du descriptif XML
    Load("data/window/option.xml");
    volume_ = 100;
}


Option::~Option()
{
}


int Option::WindowCallback(Control::ControlID id)
{
    OutputD << "Option ID : " << id << lEnd;
    SoundSystem& sound = SoundSystem::GetInstance();

	switch(id)
	{
		case IDEXIT:
			return _EXIT;
		case ID_MUTE:
			sound.EnableMusic(!sound.IsMusicEnabled());
			break;
		case ID_PLUS:
			volume_ += MUSIC_QUANTUM;
			OutputD << "Augmentation du volume de musique !" << lEnd;
			sound.SetMusicVolume(volume_);
			break;
		case ID_MOINS:
			volume_ -= MUSIC_QUANTUM;
			sound.SetMusicVolume(volume_);
			OutputD << "Diminution du volume de musique !" << sound.GetMusicVolume() << lEnd;
			break;
	}
    return 0;
}
