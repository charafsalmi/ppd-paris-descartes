#include "Option.hpp"
#include "../core/SoundSystem.hpp"
#include "../misc/Log.hpp"

#define MUSIC_QUANTUM 10

using namespace gui;

Option::Option()
{
    // Chargement du descriptif XML
    Load("data/window/option.xml");

    saved_volume_ = -1;
    volume_ = 100;
}

Option::~Option()
{
}

int Option::WindowCallback(const Control::ControlID id, const int p1, void *p2)
{
    OutputD << "Option ID : " << id << lEnd;
    switch(id){
        case IDEXIT:
            return _EXIT;
            break;
        case ID_MUTE:
            if(saved_volume_ == -1){
                saved_volume_ = SoundSystem::GetInstance().GetMusicVolume();
                SoundSystem::GetInstance().SetMusicVolume(0.0f);
                OutputD << "Saved volume : " << saved_volume_ << lEnd;
            }else{
                OutputD << "Restored volume : " << saved_volume_ << lEnd;
                SoundSystem::GetInstance().SetMusicVolume(saved_volume_);
                saved_volume_ = -1;
            }
            break;
        case ID_PLUS:
            volume_ += MUSIC_QUANTUM;
            OutputD << "Augmentation du volume de musique !" << lEnd;
            SoundSystem::GetInstance().SetMusicVolume(volume_);
            break;
        case ID_MOINS:
            volume_ -= MUSIC_QUANTUM;
            SoundSystem::GetInstance().SetMusicVolume(volume_);
            OutputD << "Diminution du volume de musique !" << SoundSystem::GetInstance().GetMusicVolume() << lEnd;
            break;
    }
    return 0;
}
