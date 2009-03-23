#ifndef SOUNDSYSTEM_HPP
#define SOUNDSYSTEM_HPP

#include <string>
#include <SFML/Audio.hpp>

class SoundSystem
{
public:
	static SoundSystem& GetInstance();

	/**
	 * Jouer un son
	 */
	void PlaySound(const char* sound_name);

	/**
	 * Jouer une musique (une musique Ã  la fois)
	 */
	void PlayMusic(const char* music_name);
	void PlayMusic(const std::string& music_name);

	/**
	 * Stopper la lecture de la musique
	 */
	void StopMusic();

	/**
	 * Activer/Desactiver la musique
	 */
	void EnableMusic(bool enabled);

	/**
	 * Changer le volume du son (de 0 ˆ 100)
	 */
	inline void SetMusicVolume(float v)
	{
		volume_ = v;
		music_->SetVolume(v);
	}
	/**
	 * Obtenir le volume du son (de 0 ˆ 100)
	 */
	inline float GetMusicVolume() const
	{
		return volume_;
	}

	/**
	 * Changer le volume du son de facon douce (fondu)
	 * @param[in] float nouveau volume
	 * @param[in] float delay (en secondes)
	 */
	inline void FadeMusicVolumeTo(const float& to_volume, float delay)
	{
		timer_ = 0;
		update_volume_to_ = to_volume;
		fade_delay_ = delay;
		std::cout << "[SoundSystem] Fondu de volume de " << volume_ << "% a " << to_volume << "% en " << delay << "s" << std::endl;
	}

	/**
	 * Mise a jour du volume
	 */
	void UpdateVolume(float frametime);

private:
	SoundSystem();
	SoundSystem(const SoundSystem&);
	~SoundSystem();

	enum {MAX_SOUNDS = 20};

	sf::Sound sounds_[MAX_SOUNDS];
	int last_used_;
	sf::Music* music_;
	bool enable_music_;
	float timer_;
	float volume_;
	float update_volume_to_;
	float fade_delay_;
};


#endif /* SOUNDSYSTEM_HPP */
